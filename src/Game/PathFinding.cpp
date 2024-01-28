#include "PathFinding.h"

#include "CPosition.h"
#include "CTileMap.h"
#include "TIsSolid.h"
#include "TIsTile.h"
#include "Utility.h"
#include <raymath.h>

// StepTile{position, direction_it_was_accessed_from}
using SteppedTile = std::pair<Vector2, Vector2>;

// Returns accessible positions (non-solid tiles)
std::vector<Vector2> filterTilesAccessible(snd::ECS* ecs)
{
    std::vector<Vector2> filteredPositions{};
    auto* tileMap{ecs->getComponent<CTileMap>(ecs->getFirstEntityWith<CTileMap>())->getTileMap()};

    for (const auto& entityId : *tileMap->getAllElements())
    {
        if (ecs->getComponent<TIsSolid>(entityId))
            continue;

        filteredPositions.push_back(ecs->getComponent<CPosition>(entityId)->getPosition());
    }

    return filteredPositions;
}

std::vector<Vector2> filterTilesInRange();

bool isTilePassable(const Vector2& tile, const std::unordered_set<snd::EntityId>* impassableTiles, snd::ECS* ecs)
{
    for (auto impassableTile : *impassableTiles)
    {
        auto impassableTilePosition{ecs->getComponent<CPosition>(impassableTile)->getPosition()};

        // If positions match aka. is impassable: continue
        if (Vector2Equals(
                tile,
                impassableTilePosition))
            return false;
    }
    return true;
}
// Pathfinder returns vector of positions from origin to target if target is in range
std::vector<Vector2> findPath(const Vector2& origin, const Vector2& target, size_t range, snd::ECS* ecs)
{
    std::vector<Vector2> path{origin};

    // Check if target equals root position
    if (Vector2Equals(target, origin))
        return path;

    // Check if range is 0
    if (!range)
        return path;

    auto* impassableTiles{ecs->getAllEntitiesWith<TIsSolid>()};

    // Check if target is impassable
    for (auto impassableTile : *impassableTiles)
    {
        if (Vector2Equals(target, ecs->getComponent<CPosition>(impassableTile)->getPosition()))
            return path;
    }

    // Start finding steps for path
    bool pathFound = false;

    std::vector<std::vector<SteppedTile>> steppedTiles{};

    // Step 0
    // Extend vector by one additional step level
    steppedTiles.push_back(std::vector<SteppedTile>());
    // Add starting position
    steppedTiles[0].push_back(SteppedTile(origin, NODIR));

    // Step 1
    // Extend vector by one additional step level
    steppedTiles.push_back(std::vector<SteppedTile>());

    // Test all four directions for first step
    for (Vector2 direction : {
             V_RIGHT,
             V_DOWN,
             V_LEFT,
             V_UP,
         })
    {
        if (pathFound) break;

        // Set next stepped tile position
        auto nextTilePosition{Vector2Add(origin, direction)};

        // Check if next tile is passable
        if (!isTilePassable(nextTilePosition, impassableTiles, ecs))
            continue;

        // Add stepped tile to container
        steppedTiles[1].push_back(std::make_pair(nextTilePosition, direction));

        // Check if stepped tile is equal to target
        if (Vector2Equals(
                nextTilePosition,
                target))
        {
            pathFound = true;
        }
    }

    // Step 2+
    for (size_t stepLevel{2}; stepLevel <= range; ++stepLevel)
    {
        if (pathFound) break;

        // Extend vector by one additional step level
        steppedTiles.push_back(std::vector<SteppedTile>());

        size_t previousStepLevel{stepLevel - 1};
        size_t previousTilesCount{steppedTiles[previousStepLevel].size()};

        // Iterate previous stepped tiles to step one further from
        for (size_t tileIndex{0}; tileIndex < previousTilesCount; ++tileIndex)
        {
            if (pathFound) break;

            SteppedTile steppedTile{steppedTiles[previousStepLevel][tileIndex]};

            // Check the 3 neighbours it was not stepped from
            for (Matrix2x2 R : {
                     M_ROTATE_NONE,
                     M_ROTATE_LEFT,
                     M_ROTATE_RIGHT})
            {
                if (pathFound) break;

                // Set next stepped tile position
                Vector2 nextTilePosition{Vector2Add(steppedTile.first, Vector2MatrixMultiply(R, steppedTile.second))};

                // Check if tile is already known
                bool tileKnown{false};

                for (auto tile : steppedTiles[stepLevel])
                {
                    if (!Vector2Equals(tile.first, nextTilePosition))
                        continue;

                    tileKnown = true;
                    break;
                }

                if (tileKnown) continue;

                // Check if next tile is impassable
                if (!isTilePassable(nextTilePosition, impassableTiles, ecs))
                    continue;

                // Add passable tile to stepped tiles
                steppedTiles[stepLevel].push_back(std::make_pair(nextTilePosition, Vector2MatrixMultiply(R, steppedTile.second)));

                // Check if stepped tile is equal to target
                if (Vector2Equals(
                        nextTilePosition,
                        target))
                {
                    pathFound = true;
                }
            }
        }
    }

    // Return empty path if no path found
    if (!pathFound) return path;

    // Reconstruct path from steps
    size_t stepsNeeded{steppedTiles.size() - 1};
    // Set found target tile
    Vector2 currentStepLevelTile{steppedTiles.back().back().first};

    // Replace dummy null position with final position to path
    path.insert(path.begin()++, currentStepLevelTile);

    // Insert tile from previous step level to the front of path if its neighbour to current step level tile (initialy this is the target)
    for (auto stepLevel{stepsNeeded - 1}; stepLevel > 0; --stepLevel)
    {
        // Check tiles from previous step level if its neighbour to current step level tile
        for (auto tile : steppedTiles[stepLevel])
        {
            Vector2 checkVector{Vector2Subtract(tile.first, currentStepLevelTile)};
            auto checkValue{abs(checkVector.x) + abs(checkVector.y)};

            // If subtracted Vectors have one coordinate 0 and the other +/- 1 then they are neighbours
            if (checkValue == 1)
            {
                path.insert(path.begin()++, tile.first);
                currentStepLevelTile = tile.first;
            }
        }
    }

    // Return path (has only one element if no path found)
    return path;
}
