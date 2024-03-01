#include "Pathfinder.h"

#include "Constants.h"
#include "RuntimeDatabase.h"
#include "TileMap.h"
#include <raylibEx.h>

// StepTile{position, direction_it_was_accessed_from}
bool isInPositions(
    const Vector2i& target,
    const std::vector<Vector2i>& positions)
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

bool isInTiles(
    const Vector2i& target,
    const std::vector<SteppedPosition>& tiles)
{
    for (const auto& tile : tiles)
    {
        if (Vector2Equals(target, tile.tilePosition))
            return true;
    }
    return false;
}

bool isInSteppedTiles(
    const Vector2i& target,
    const ClassifiedPositions& steppedTiles)
{
    for (const auto& tiles : steppedTiles)
    {
        for (const auto& tile : tiles)
        {
            if (Vector2Equals(
                    target,
                    tile.tilePosition))
                return true;
        }
    }
    return false;
}

// Returns accessible positions (non-solid tiles)
std::vector<Vector2i> filterNonSolidPositions(TileMap& tileMap)
{
    std::vector<Vector2i> accessiblePositions{};

    for (auto& tile : tileMap.values())
    {
        if (tile.isSolid)
            continue;

        accessiblePositions.push_back(tile.tilePosition);
    }

    return accessiblePositions;
}

std::vector<Vector2i> filterInRange(
    const std::vector<Vector2i>& nonSolidPositions,
    size_t range,
    const Vector2i& origin)
{
    std::vector<Vector2i> inRangePositions{};

    // Check if range is 0
    if (!range)
        return inRangePositions;

    for (const auto& position : nonSolidPositions)
    {
        Vector2i delta{Vector2Subtract(position, origin)};

        if (Vector2Length(delta) > range)
            continue;

        inRangePositions.push_back(position);
    }

    return inRangePositions;
}

std::vector<Vector2i> filterInRange(
    TileMap& tileMap,
    size_t range,
    const Vector2i& origin)
{
    return filterInRange(filterNonSolidPositions(tileMap), range, origin);
}

bool isInRange(
    const Vector2i& target,
    size_t range,
    const Vector2i& origin,
    TileMap& tileMap)
{
    return isInPositions(target, filterInRange(tileMap, range, origin));
}

ClassifiedPositions filterReachable(
    const std::vector<Vector2i>& inRangePositions,
    size_t range,
    const Vector2i& origin)
{
    ClassifiedPositions steppedPositions{};

    // Check if range is 0
    if (!range)
        return steppedPositions;

    // Step 0

    // Extend vector by one additional step level
    steppedPositions.push_back(std::vector<SteppedPosition>());

    // Add starting position
    steppedPositions.front().push_back(SteppedPosition(origin));

    // Step 1

    // Extend vector by one additional step level
    steppedPositions.push_back(std::vector<SteppedPosition>());

    // Test all four directions for first step
    for (Vector2i direction : {
             V_RIGHT,
             V_DOWN,
             V_LEFT,
             V_UP,
         })
    {
        // Set next stepped tile position
        auto nextTilePosition{Vector2Add(origin, direction)};

        // Check if next tile is in range
        if (!isInPositions(nextTilePosition, inRangePositions))
            continue;

        // Add stepped tile to stepped tiles
        steppedPositions.back().push_back(SteppedPosition(nextTilePosition, direction));
    }

    // Steps > 1

    for (size_t stepLevel{2}; stepLevel <= range; ++stepLevel)
    {
        // Extend vector by one additional step level
        steppedPositions.push_back(std::vector<SteppedPosition>());

        size_t previousStepLevel{stepLevel - 1};
        size_t previousTilesCount{steppedPositions[previousStepLevel].size()};

        // Iterate previous stepped tiles to step one further from
        for (size_t tileIndex{0}; tileIndex < previousTilesCount; ++tileIndex)
        {
            SteppedPosition steppedTile{steppedPositions[previousStepLevel][tileIndex]};

            // Check the 3 neighbours it was not stepped from
            for (Matrix2i R : {M_ROTATE_NONE, M_ROTATE_LEFT, M_ROTATE_RIGHT})
            {
                // Set next stepped tile position
                Vector2i nextTilePosition{Vector2Add(steppedTile.tilePosition, MatrixMultiply(R, steppedTile.direction))};

                // Check if tile is already known
                bool tileKnown{false};

                for (const auto& tiles : steppedPositions)
                {
                    for (const auto& tile : tiles)
                    {
                        if (!Vector2Equals(tile.tilePosition, nextTilePosition))
                            continue;

                        tileKnown = true;
                        break;
                    }
                }

                if (tileKnown) continue;

                // Check if next tile is in range
                if (!isInPositions(nextTilePosition, inRangePositions))
                    continue;

                // Add passable tile to stepped tiles
                steppedPositions[stepLevel].push_back(SteppedPosition(nextTilePosition, MatrixMultiply(R, steppedTile.direction)));
            }
        }
        if (steppedPositions[stepLevel].empty())
        {
            steppedPositions.pop_back();
            break;
        }
    }

    return steppedPositions;
}

ClassifiedPositions filterReachable(
    TileMap& tileMap,
    size_t range,
    const Vector2i& origin)
{
    return filterReachable(filterInRange(tileMap, range, origin), range, origin);
}

bool isReachable(
    const Vector2i& target,
    size_t range,
    const Vector2i& origin,
    TileMap& tileMap)
{
    return isInSteppedTiles(target, filterReachable(tileMap, range, origin));
}

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
std::vector<SteppedPosition> findPath(
    const ClassifiedPositions& reachableTiles,
    const Vector2i& target)
{
    std::vector<SteppedPosition> path{};

    // Check if target is reachable
    if (!isInSteppedTiles(target, reachableTiles))
        return path;

    Vector2i origin{reachableTiles.front().front().tilePosition};

    // Check if target equals root position
    if (Vector2Equals(target, origin))
        return path;

    // Find tiles in reachable positions, and recursively find neighbour until origin in next lower step level
    SteppedPosition currentStepLevelTile{};
    size_t stepsNeeded{};
    size_t maxRange{reachableTiles.size()};

    // Check if range is 0
    if (!maxRange)
        return path;

    // Find target tile in reachable positions
    for (size_t stepLevel{0}; stepLevel < maxRange; ++stepLevel)
    {
        for (const auto& tile : reachableTiles[stepLevel])
        {
            if (!Vector2Equals(tile.tilePosition, target))
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
            Vector2i checkVector{Vector2Subtract(currentStepLevelTile.tilePosition, tile.tilePosition)};

            // Tiled length of checkVector needs to be 1 (then it is a neighbour)
            auto checkValue{Vector2Length(checkVector)};

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

std::vector<SteppedPosition> findPath(
    TileMap& tileMap,
    const Vector2i& origin,
    const Vector2i& target,
    size_t range)
{
    return findPath(filterReachable(tileMap, range, origin), target);
}