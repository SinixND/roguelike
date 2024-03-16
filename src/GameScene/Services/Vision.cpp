#include "Vision.h"

#include "RayCast.h"
#include "Tile.h"
#include "TileMap.h"
#include "TileMapFilters.h"
#include "Unit.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

namespace
{
    void resetVisibleTiles(std::vector<Tile*>& tiles);
    void addRayTargets(std::vector<Vector2i>& rayTargets, Unit& unit);
}

namespace Vision
{
    void update(Unit& unit, TileMap& tileMap)
    {
        // Filter tiles
        auto tilesInExtendedVisionRange{
            TileMapFilters::filterInRange(
                tileMap,
                unit.visionRange + 1,
                unit.position.tilePosition())};

        // Reset "visible" tiles to "seen" in extended vision range
        resetVisibleTiles(tilesInExtendedVisionRange);

        // Set ray targets
        std::vector<Vector2i> rayTargets{};

        addRayTargets(rayTargets, unit);

        // Set visible tiles
        std::vector<Tile*> visibleTiles{RayCast::getTilesRayed(rayTargets, unit.position.get(), tileMap)};

        // Make tiles visible
        for (auto& tile : visibleTiles)
        {
            tile->visibilityID = VisibilityID::VISIBLE;
        }
    }
}

namespace
{
    void resetVisibleTiles(std::vector<Tile*>& tiles)
    {
        for (auto& tile : tiles)
        {
            if (tile->visibilityID == VisibilityID::VISIBLE)
            {
                tile->visibilityID = VisibilityID::SEEN;

                //* if (dtb::debugMode())
                //* {
                //* BeginDrawing();
                //* BeginMode2D(dtb::camera());

                //* DrawCircleLinesV(tile->position.get(), 5, RED);

                //* EndMode2D();
                //* EndDrawing();
                //* }
            }
        }
    }

    void addRayTargets(std::vector<Vector2i>& rayTargets, Unit& unit)
    {
        Vector2i origin{unit.position.tilePosition()};
        int visionRange{unit.visionRange};

        // Add all target positions that are exaclty at vision range
        // Set first target
        Vector2i firstTarget{
            Vector2Add(
                origin,
                Vector2i{
                    visionRange,
                    0})};

        Vector2i target{firstTarget};

        // Loop positions
        Vector2i delta{-1, +1};

        do
        {
            // Add target
            rayTargets.push_back(target);

            // Update target
            target = Vector2Add(target, delta);

            // Adjust / flip delta
            if ((target.x >= (origin.x + visionRange)) ||
                (target.x <= (origin.x - visionRange)))
                delta.x *= -1;

            if ((target.y >= (origin.y + visionRange)) ||
                (target.y <= (origin.y - visionRange)))
                delta.y *= -1;

        } while (!Vector2Equals(target, firstTarget));
    }
}
