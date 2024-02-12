#include "PathFinding.h"

#include "Utility.h"
#include <cassert>
#include <cstddef>
#include <raylibEx.h>
#include <raymath.h>

// StepTile{position, direction_it_was_accessed_from}
bool isPositionInPositions(const Vector2Int& target, const std::vector<Vector2Int>& positions)
{
    for (auto position : positions)
    {
        if (Vector2Equals(
                target,
                position))
            return true;
    }
    return false;
}

bool isInTileList(const Vector2Int& target, const std::vector<SteppedTile>& tiles)
{
    for (const auto& tile : tiles)
    {
        if (Vector2Equals(target, tile.position))
            return true;
    }
    return false;
}

bool isPositionInSteppedTiles(const Vector2Int& target, const std::vector<std::vector<SteppedTile>>& steppedTiles)
{
    for (const auto& tiles : steppedTiles)
    {
        for (const auto& tile : tiles)
        {
            if (Vector2Equals(
                    target,
                    tile.position))
                return true;
        }
    }
    return false;
}

/*
// Returns accessible positions (non-solid tiles)
std::vector<Vector2Int> filterTilesAccessible(snd::ECS* ecs)
{
    std::vector<Vector2Int> accessiblePositions{};
    auto* tileMap{ecs->getComponent<CTileMap>(ecs->getFirstEntityWith<CTileMap>())->getTileMap()};

    for (const auto& entityId : *tileMap->getAllElements())
    {
        if (ecs->getComponent<TIsSolid>(entityId))
            continue;

        accessiblePositions.push_back(ecs->getComponent<CPosition>(entityId)->getPosition());
    }

    return accessiblePositions;
}

bool isTileAccessible(const Vector2Int& target, snd::ECS* ecs)
{
    return isPositionInPositions(target, filterTilesAccessible(ecs));
}

std::vector<Vector2Int> filterTilesInRange(const Vector2Int& origin, size_t range, const std::vector<Vector2Int>& accessiblePositions)
{
    std::vector<Vector2Int> inRangePositions{};

    // Check if range is 0
    if (!range)
        return inRangePositions;

    for (const auto& position : accessiblePositions)
    {
        Vector2Int delta{Vector2Subtract(position, origin)};

        if (Vector2LengthTiled(delta) > range)
            continue;

        inRangePositions.push_back(position);
    }

    return inRangePositions;
}

std::vector<Vector2Int> filterTilesInRange(const Vector2Int& origin, size_t range, snd::ECS* ecs)
{
    return filterTilesInRange(origin, range, filterTilesAccessible(ecs));
}

bool isTileInRange(const Vector2Int& origin, const Vector2Int& target, size_t range, snd::ECS* ecs)
{
    return isPositionInPositions(target, filterTilesInRange(origin, range, ecs));
}

std::vector<std::vector<SteppedTile>> filterTilesReachable(const Vector2Int& origin, size_t range, const std::vector<Vector2Int>& inRangePositions)
{

    std::vector<std::vector<SteppedTile>> steppedTiles{};

    // Check if range is 0
    if (!range)
        return steppedTiles;

    // Step 0

    // Extend vector by one additional step level
    steppedTiles.push_back(std::vector<SteppedTile>());

    // Add starting position
    steppedTiles[0].push_back(SteppedTile(origin));

    // Step 1

    // Extend vector by one additional step level
    steppedTiles.push_back(std::vector<SteppedTile>());

    // Test all four directions for first step
    for (Vector2Int direction : {
             V_RIGHT,
             V_DOWN,
             V_LEFT,
             V_UP,
         })
    {
        // Set next stepped tile position
        auto nextTilePosition{Vector2Add(origin, direction)};

        // Check if next tile is in range
        if (!isPositionInPositions(nextTilePosition, inRangePositions))
            continue;

        // Add stepped tile to stepped tiles
        steppedTiles[1].push_back(SteppedTile(nextTilePosition, direction));
    }

    // Steps > 1

    for (size_t stepLevel{2}; stepLevel <= range; ++stepLevel)
    {
        // Extend vector by one additional step level
        steppedTiles.push_back(std::vector<SteppedTile>());

        size_t previousStepLevel{stepLevel - 1};
        size_t previousTilesCount{steppedTiles[previousStepLevel].size()};

        // Iterate previous stepped tiles to step one further from
        for (size_t tileIndex{0}; tileIndex < previousTilesCount; ++tileIndex)
        {
            SteppedTile steppedTile{steppedTiles[previousStepLevel][tileIndex]};

            // Check the 3 neighbours it was not stepped from
            for (Matrix2x2Int R : {
                     M_ROTATE_NONE,
                     M_ROTATE_LEFT,
                     M_ROTATE_RIGHT})
            {
                // Set next stepped tile position
                Vector2Int nextTilePosition{Vector2Add(steppedTile.position, Vector2MatrixMultiply(R, steppedTile.direction))};

                // Check if tile is already known
                bool tileKnown{false};

                for (const auto& tiles : steppedTiles)
                {
                    for (const auto& tile : tiles)
                    {
                        if (!Vector2Equals(tile.position, nextTilePosition))
                            continue;

                        tileKnown = true;
                        break;
                    }
                }

                if (tileKnown) continue;

                // Check if next tile is in range
                if (!isPositionInPositions(nextTilePosition, inRangePositions))
                    continue;

                // Add passable tile to stepped tiles
                steppedTiles[stepLevel].push_back(SteppedTile(nextTilePosition, Vector2MatrixMultiply(R, steppedTile.direction)));
            }
        }
        if (steppedTiles[stepLevel].empty())
        {
            steppedTiles.pop_back();
            break;
        }
    }

    return steppedTiles;
}

std::vector<std::vector<SteppedTile>> filterTilesReachable(const Vector2Int& origin, size_t range, snd::ECS* ecs)
{
    return filterTilesReachable(origin, range, filterTilesInRange(origin, range, ecs));
}

bool isTileReachable(const Vector2Int& origin, const Vector2Int& target, size_t range, snd::ECS* ecs)
{
    return isPositionInSteppedTiles(target, filterTilesReachable(origin, range, ecs));
}

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
std::vector<SteppedTile> findPath(const Vector2Int& origin, const Vector2Int& target, size_t range, const std::vector<std::vector<SteppedTile>>& reachableTiles)
{
    std::vector<SteppedTile> path{};

    // Check if target is reachable
    if (!isPositionInSteppedTiles(target, reachableTiles))
        return path;

    // Check if target equals root position
    if (Vector2Equals(target, origin))
        return path;

    // Check if range is 0
    if (!range)
        return path;

    // Find tiles in reachable positions, and recursively find neighbour until origin in next lower step level
    SteppedTile currentStepLevelTile{};
    size_t stepsNeeded{};
    auto reachableTilesSize{reachableTiles.size()};

    // Find target tile in reachable positions
    for (size_t stepLevel{0}; stepLevel < reachableTilesSize; ++stepLevel)
    {
        for (const auto& tile : reachableTiles[stepLevel])
        {
            if (!Vector2Equals(tile.position, target))
                continue;

            currentStepLevelTile = tile;
            stepsNeeded = stepLevel;
        }
    }

    // Reconstruct path from steps

    // Add final position to path
    currentStepLevelTile.direction = V_NODIR;
    path.push_back(currentStepLevelTile);

    // Push back position from previous step level to the path if it is a neighbour to current step level tile (which is the target initially)
    // Iterate all tiles that need one step less aka. are in previous step level
    for (auto stepLevel{stepsNeeded - 1}; stepLevel > 0; --stepLevel)
    {
        // Check tiles from previous step level if its neighbour to current step level tile
        for (auto tile : reachableTiles[stepLevel])
        {
            // CheckVector is delta between checked tiles position and current step level position
            // CheckVector also is used to store the direction to next path tile
            Vector2Int checkVector{Vector2Subtract(currentStepLevelTile.position, tile.position)};

            // Tiled length of checkVector needs to be 1 (then it is a neighbour)
            auto checkValue{Vector2LengthTiled(checkVector)};

            // Check if tile is neighbour
            if (checkValue == 1)
            {
                // Add direction to next path tile to found tile
                tile.direction = checkVector;
                path.insert(path.begin(), tile);
                currentStepLevelTile = tile;
            }
        }
    }

    return path;
}

std::vector<SteppedTile> findPath(const Vector2Int& origin, const Vector2Int& target, size_t range, snd::ECS* ecs)
{
    return findPath(origin, target, range, filterTilesReachable(origin, range, ecs));
}
*/