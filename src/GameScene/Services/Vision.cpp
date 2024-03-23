#include "Vision.h"

#include "RayCast.h"
#include "Tile.h"
#include "TileMap.h"
#include "TileMapFilters.h"
#include "Unit.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include <vector>

namespace
{
    void resetVisibleTiles(std::vector<Tile*>& tiles)
    {
        for (auto& tile : tiles) {
            if (tile->visibilityID() == VisibilityID::VISIBLE) {
                tile->setVisibilityID(VisibilityID::SEEN);

                //* if (dtb::debugMode())
                //* {
                //* BeginDrawing();
                //* BeginMode2D(dtb::camera());
                //* // Draw tile that got reset
                //* DrawCircleLinesV(tile->position().get(), 5, RED);
                //* EndMode2D();
                //* EndDrawing();
                //* }
            }
        }
    }

    //* // Add all within vision range
    //* void addRayTargetsWithinRange(std::vector<Vector2i>& rayTargets, Unit& unit,
    // TileMap& tileMap) {
    //* // Add all positions within vision range + 1
    //* for (auto& tile : TileMapFilters::filterInRange(tileMap,
    // unit.visionRange(), unit.transform.tilePosition())) {
    //* rayTargets.push_back(tile->transform.tilePosition());
    //* }
    //* }

    void addRayTargetsAtVisionRange(std::vector<Vector2i>& rayTargets, Unit& unit)
    {
        Vector2i origin{unit.transform.tilePosition()};
        size_t visionRange{unit.visionRange()};

        // Add all target positions that are exaclty at vision range
        // Set first target
        Vector2i firstTarget{
          Vector2Add(origin, Vector2i{static_cast<int>(visionRange), 0})};
        Vector2i target{firstTarget};

        // Loop positions
        Vector2i delta{-1, +1};

        do {
            // Add target
            rayTargets.push_back(target);

            // Update target
            addToVector2(target, delta);

            // Adjust / flip delta
            if (
              (target.x >= (origin.x + static_cast<int>(visionRange))) ||
              (target.x <= (origin.x - static_cast<int>(visionRange))))
                delta.x *= -1;

            if (
              (target.y >= (origin.y + static_cast<int>(visionRange))) ||
              (target.y <= (origin.y - static_cast<int>(visionRange))))
                delta.y *= -1;
        } while (!Vector2Equals(target, firstTarget));
    }

    void addRayTargetsOpaque(
      std::vector<Vector2i>& rayTargets,
      Unit& unit,
      TileMap& tileMap)
    {
        Vector2i origin{unit.transform.tilePosition()};
        size_t visionRange{unit.visionRange()};

        // Add all opaque positions within vision range
        for (auto& tile : TileMapFilters::filterOpaqueTiles(tileMap)) {
            Vector2i target{tile->transform.tilePosition()};

            if (Vector2Sum(Vector2Subtract(target, origin)) > visionRange)
                continue;
            rayTargets.push_back(target);
        }
    }

}

namespace Vision
{
    void update(Unit& unit, TileMap& tileMap)
    {

        // Filter tiles
        auto tilesInExtendedVisionRange{TileMapFilters::filterInRange(
          tileMap,
          unit.visionRange() + 1,
          unit.transform.tilePosition())};

        // Reset "visible" tiles to "seen" in extended vision range
        resetVisibleTiles(tilesInExtendedVisionRange);

        // Set ray targets
        static std::vector<Vector2i> rayTargets{};

        if (unit.transform.hasPositionChanged()) {
            rayTargets.clear();
            addRayTargetsOpaque(rayTargets, unit, tileMap);
            addRayTargetsAtVisionRange(rayTargets, unit);
        }

        // Set visible tiles
        static std::vector<Tile*> visibleTiles{};

        if (unit.transform.hasPositionChanged())
            visibleTiles = RayCast::getTilesRayed(
              rayTargets,
              unit.transform.position(),
              tileMap);

        // Make tiles visible
        for (auto& tile : visibleTiles) {
            tile->setVisibilityID(VisibilityID::VISIBLE);
        }
    }

}
