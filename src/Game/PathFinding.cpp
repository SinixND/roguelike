#include "PathFinding.h"

#include "CPosition.h"
#include "CTileMap.h"
#include "ECS.h"
#include "TIsSolid.h"
#include "TIsTile.h"
#include "Utility.h"
#include <cassert>
#include <cstddef>
#include <raylib.h>
#include <raymath.h>

// StepTile{position, direction_it_was_accessed_from}
//* using SteppedTile = std::pair<Vector2, Vector2>;
struct SteppedTile
{
    Vector2 position;
    Vector2 direction = V_NODIR; // in which it was accessed
    size_t steps = 0;            // needed to reach
};

bool isPositionInList(const Vector2& target, const std::vector<Vector2>& positions)
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

// Returns accessible positions (non-solid tiles)
std::vector<Vector2> filterTilesAccessible(snd::ECS* ecs)
{
    std::vector<Vector2> accessiblePositions{};
    auto* tileMap{ecs->getComponent<CTileMap>(ecs->getFirstEntityWith<CTileMap>())->getTileMap()};

    for (const auto& entityId : *tileMap->getAllElements())
    {
        if (ecs->getComponent<TIsSolid>(entityId))
            continue;

        accessiblePositions.push_back(ecs->getComponent<CPosition>(entityId)->getPosition());
    }

    return accessiblePositions;
}

bool isTileAccessible(const Vector2& target, snd::ECS* ecs)
{
    return isPositionInList(target, filterTilesAccessible(ecs));
}

std::vector<Vector2> filterTilesInRange(const Vector2& origin, size_t range, const std::vector<Vector2>& accessiblePositions)
{
    std::vector<Vector2> inRangePositions{};

    // Check if range is 0
    if (!range)
        return inRangePositions;

    for (const auto& position : accessiblePositions)
    {
        Vector2 delta{Vector2Subtract(position, origin)};

        if (Vector2LengthTiled(delta) > range)
            continue;

        inRangePositions.push_back(position);
    }

    return inRangePositions;
}

std::vector<Vector2> filterTilesInRange(const Vector2& origin, size_t range, snd::ECS* ecs)
{
    return filterTilesInRange(origin, range, filterTilesAccessible(ecs));
}

bool isTileInRange(const Vector2& origin, const Vector2& target, size_t range, snd::ECS* ecs)
{
    return isPositionInList(target, filterTilesInRange(origin, range, ecs));
}

std::vector<std::vector<Vector2>> filterTilesReachable(const Vector2& origin, size_t range, const std::vector<Vector2>& inRangePositions)
{
    std::vector<std::vector<Vector2>> reachablePositions{std::vector{std::vector{origin}}};

    // Check if range is 0
    if (!range)
        return reachablePositions;

    std::vector<std::vector<SteppedTile>> steppedTiles{};

    // Step 0

    // Extend vector by one additional step level
    steppedTiles.push_back(std::vector<SteppedTile>());

    // Add starting position
    steppedTiles[0].push_back(SteppedTile(origin));

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
        // Set next stepped tile position
        auto nextTilePosition{Vector2Add(origin, direction)};

        // Check if next tile is in range
        if (!isPositionInList(nextTilePosition, inRangePositions))
            continue;

        // Add stepped tile to stepped tiles
        steppedTiles[1].push_back(SteppedTile(nextTilePosition, direction));

        reachablePositions.push_back(nextTilePosition);
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
            for (Matrix2x2 R : {
                     M_ROTATE_NONE,
                     M_ROTATE_LEFT,
                     M_ROTATE_RIGHT})
            {
                // Set next stepped tile position
                Vector2 nextTilePosition{Vector2Add(steppedTile.position, Vector2MatrixMultiply(R, steppedTile.direction))};

                // Check if tile is already known
                bool tileKnown{false};

                for (auto tile : steppedTiles[previousStepLevel])
                {
                    if (!Vector2Equals(tile.position, nextTilePosition))
                        continue;

                    tileKnown = true;
                    break;
                }

                if (tileKnown) continue;

                // Check if next tile is in range
                if (!isPositionInList(nextTilePosition, inRangePositions))
                    continue;

                // Add passable tile to stepped tiles
                steppedTiles[stepLevel].push_back(SteppedTile(nextTilePosition, Vector2MatrixMultiply(R, steppedTile.direction)));

                reachablePositions.push_back(nextTilePosition);
            }
        }
    }

    return reachablePositions;
}

std::vector<Vector2> filterTilesReachable(const Vector2& origin, size_t range, snd::ECS* ecs)
{
    return filterTilesReachable(origin, range, filterTilesInRange(origin, range, ecs));
}

bool isTileReachable(const Vector2& origin, const Vector2& target, size_t range, snd::ECS* ecs)
{
    return isPositionInList(target, filterTilesReachable(origin, range, ecs));
}

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
std::vector<Vector2> findPath(const Vector2& origin, const Vector2& target, size_t range, const std::vector<Vector2>& reachablePositions)
{
    std::vector<Vector2> path{};

    // Check if target is reachable
    if (!isPositionInList(target, reachablePositions))
        return path;

    // Check if target equals root position
    if (Vector2Equals(target, origin))
        return path;

    // Check if range is 0
    if (!range)
        return path;

    // Start finding steps for path

    bool pathFound = false;

    std::vector<std::vector<SteppedTile>> steppedTiles{};

    // Step 0

    // Extend vector by one additional step level
    steppedTiles.push_back(std::vector<SteppedTile>());

    // Add starting position
    steppedTiles[0].push_back(SteppedTile(origin));

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

        // Check if next tile is reachable
        if (!isPositionInList(nextTilePosition, reachablePositions))
            continue;

        // Add reachable tile to stepped tiles
        steppedTiles[1].push_back(std::make_pair(nextTilePosition, direction));

        // Check if stepped tile is equal to target
        if (Vector2Equals(
                nextTilePosition,
                target))
        {
            pathFound = true;
        }
    }

    // Steps > 1

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

                // Check if next position is reachable
                if (!isPositionInList(nextTilePosition, reachablePositions))
                    continue;

                // Add reachable tile to stepped tiles
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

    // If no path found, there is an error in the code
    assert(pathFound && "ERROR: NO PATH FOUND ALTHOUGH TARGET IS REACHABLE!");

    // Reconstruct path from steps
    size_t stepsNeeded{steppedTiles.size() - 1};

    // Set found target position
    Vector2 currentStepLevelPosition{steppedTiles.back().back().first};

    // Add final position to path
    path.push_back(currentStepLevelPosition);

    // Push back position from previous step level to the path if it is a neighbour to current step level tile (which is the target initially)
    for (auto stepLevel{stepsNeeded - 1}; stepLevel > 0; --stepLevel)
    {
        // Check tiles from previous step level if its neighbour to current step level tile
        for (auto tile : steppedTiles[stepLevel])
        {
            // CheckVector is delta between checked tiles position and current step level position
            Vector2 checkVector{Vector2Subtract(tile.first, currentStepLevelPosition)};

            // Tiled length of checkVector needs to be 1 (then it is a neighbour)
            auto checkValue{Vector2LengthTiled(checkVector)};

            // Check if tile is neighbour
            if (checkValue == 1)
            {
                path.push_back(tile.first);
                currentStepLevelPosition = tile.first;
            }
        }
    }

    return path;
}

std::vector<Vector2> findPath(const Vector2& origin, const Vector2& target, size_t range, snd::ECS* ecs)
{
    return findPath(origin, target, range, filterTilesReachable(origin, range, ecs));
}
