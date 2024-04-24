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
    void resetVisibleTiles(std::vector<Tile*>& tiles)
    {
        for (auto& tile : tiles)
        {
            if (tile->visibilityID() == VisibilityID::VISIBLE)
            {
                tile->setVisibilityID(VisibilityID::SEEN);
            }
        }
    }

    [[maybe_unused]] void addRayTargetsAtVisionRange(std::vector<Vector2I>& rayTargets, Unit& unit)
    {
        Vector2I origin{unit.positionComponent.tilePosition()};
        int visionRange{unit.visionRange()};

        // Add all target positions that are exaclty at vision range
        // Set first target
        Vector2I firstTarget{
            Vector2Add(
                origin,
                Vector2I{
                    visionRange,
                    0})};

        Vector2I target{firstTarget};

        // Loop positions
        Vector2I delta{-1, +1};

        do
        {
            // Add target
            rayTargets.push_back(target);

            // Update target
            target += delta;

            // Adjust / flip delta
            if (
                (target.x >= (origin.x + visionRange))
                || (target.x <= (origin.x - visionRange)))
            {
                delta.x *= -1;
            }

            if (
                (target.y >= (origin.y + visionRange))
                || (target.y <= (origin.y - visionRange)))
            {
                delta.y *= -1;
            }
        } while (!Vector2Equals(target, firstTarget));
    }

    [[maybe_unused]] void addRayTargetsOpaque(
        std::vector<Vector2I>& rayTargets,
        Unit& unit,
        TileMap& tileMap)
    {
        Vector2I origin{unit.positionComponent.tilePosition()};
        int visionRange{unit.visionRange()};

        // Add all opaque positions within vision range
        for (auto& tile : TileMapFilters::filterOpaqueTiles(tileMap))
        {
            Vector2I target{tile->positionComponent.tilePosition()};

            if (Vector2Sum(Vector2Subtract(target, origin)) > visionRange)
            {
                continue;
            }

            rayTargets.push_back(target);
        }
    }

    // Add all within vision range
    [[maybe_unused]] void addRayTargetsWithinRange(std::vector<Vector2I>& rayTargets, Unit& unit, TileMap& tileMap)
    {
        // Add all positions within vision range + 1
        for (auto& tile : TileMapFilters::filterInRange(tileMap, unit.visionRange(), unit.positionComponent.tilePosition()))
        {
            rayTargets.push_back(tile->positionComponent.tilePosition());
        }
    }

}

namespace Vision
{
    void update(Unit& unit, TileMap& tileMap)
    {
        // Filter tiles
        auto tilesInExtendedVisionRange{
            TileMapFilters::filterInRange(
                tileMap,
                unit.visionRange() + 1,
                unit.positionComponent.tilePosition())};

        // Reset "visible" tiles to "seen" in extended vision range
        resetVisibleTiles(tilesInExtendedVisionRange);

        // Set ray targets if unit position has changed
        std::vector<Vector2I> rayTargets{};

        //* addRayTargetsOpaque(rayTargets, unit, tileMap);
        //* addRayTargetsAtVisionRange(rayTargets, unit);
        addRayTargetsWithinRange(rayTargets, unit, tileMap);

        // Set visible tiles if unit position has changed
        std::vector<Tile*> visibleTiles{};

        visibleTiles = RayCast::getTilesRayed(
            rayTargets,
            unit.positionComponent.tilePosition(),
            tileMap);

        // Make tiles visible
        for (auto& tile : visibleTiles)
        {
            tile->setVisibilityID(VisibilityID::VISIBLE);
        }
    }
}
