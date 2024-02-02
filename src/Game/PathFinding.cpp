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

bool isPositionInSteppedTiles(const Vector2& target, const std::vector<std::vector<SteppedTile>>& steppedTiles)
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

std::vector<std::vector<SteppedTile>> filterTilesReachable(const Vector2& origin, size_t range, const std::vector<Vector2>& inRangePositions)
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
                if (!isPositionInList(nextTilePosition, inRangePositions))
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

std::vector<std::vector<SteppedTile>> filterTilesReachable(const Vector2& origin, size_t range, snd::ECS* ecs)
{
    return filterTilesReachable(origin, range, filterTilesInRange(origin, range, ecs));
}

bool isTileReachable(const Vector2& origin, const Vector2& target, size_t range, snd::ECS* ecs)
{
    return isPositionInSteppedTiles(target, filterTilesReachable(origin, range, ecs));
}

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
std::vector<Vector2> findPath(const Vector2& origin, const Vector2& target, size_t range, const std::vector<std::vector<SteppedTile>>& reachablePositions)
{
    std::vector<Vector2> path{};

    // Check if target is reachable
    if (!isPositionInSteppedTiles(target, reachablePositions))
        return path;

    // Check if target equals root position
    if (Vector2Equals(target, origin))
        return path;

    // Check if range is 0
    if (!range)
        return path;

    // Start finding target position and steps needed
    Vector2 currentStepLevelPosition{};
    size_t stepsNeeded{};

    auto reachablePositionsSize{reachablePositions.size()};
    for (size_t stepLevel{0}; stepLevel < reachablePositionsSize; ++stepLevel)
    {
        for (const auto& tile : reachablePositions[stepLevel])
        {
            if (!Vector2Equals(tile.position, target))
                continue;

            currentStepLevelPosition = tile.position;
            stepsNeeded = stepLevel;
        }
    }

    // Reconstruct path from steps

    // Add final position to path
    path.push_back(currentStepLevelPosition);

    // Push back position from previous step level to the path if it is a neighbour to current step level tile (which is the target initially)
    for (auto stepLevel{stepsNeeded - 1}; stepLevel > 0; --stepLevel)
    {
        // Check tiles from previous step level if its neighbour to current step level tile
        for (auto tile : reachablePositions[stepLevel])
        {
            // CheckVector is delta between checked tiles position and current step level position
            Vector2 checkVector{Vector2Subtract(tile.position, currentStepLevelPosition)};

            // Tiled length of checkVector needs to be 1 (then it is a neighbour)
            auto checkValue{Vector2LengthTiled(checkVector)};

            // Check if tile is neighbour
            if (checkValue == 1)
            {
                path.insert(path.begin(), tile.position);
                currentStepLevelPosition = tile.position;
            }
        }
    }

    return path;
}

std::vector<Vector2> findPath(const Vector2& origin, const Vector2& target, size_t range, snd::ECS* ecs)
{
    return findPath(origin, target, range, filterTilesReachable(origin, range, ecs));
}
