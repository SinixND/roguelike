#include "TileMapFilters.h"

#include "Constants.h"
#include "TileMap.h"
#include <cstddef>
#include <raylibEx.h>
#include <vector>

namespace TileMapFilters
{
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
        const RangeSeparatedPositions& steppedTiles)
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

    // Returns all positions
    std::vector<Vector2i> filterAllPositions(TileMap& tileMap)
    {
        std::vector<Vector2i> positions{};

        for (auto& tile : tileMap.values())
        {
            positions.push_back(tile.position.tilePosition());
        }

        return positions;
    }

    // Returns accessible positions (non-solid tiles)
    std::vector<Vector2i> filterNonSolidPositions(TileMap& tileMap)
    {
        std::vector<Vector2i> accessiblePositions{};

        for (auto& tile : tileMap.values())
        {
            if (tile.isSolid)
                continue;

            accessiblePositions.push_back(tile.position.tilePosition());
        }

        return accessiblePositions;
    }

    std::vector<Vector2i> filterInRange(
        const std::vector<Vector2i>& positions,
        size_t rangeStart,
        size_t rangeEnd,
        const Vector2i& origin)
    {
        std::vector<Vector2i> inRangePositions{};

        // Check if ranges are valid
        if (rangeEnd == 0 || rangeEnd < rangeStart)
            return inRangePositions;

        for (const auto& position : positions)
        {
            Vector2i delta{Vector2Subtract(position, origin)};
            size_t sumDelta{Vector2Sum(delta)};

            if (sumDelta < rangeStart || sumDelta > rangeEnd)
                continue;

            inRangePositions.push_back(position);
        }

        return inRangePositions;
    }

    std::vector<Vector2i> filterInRange(
        const std::vector<Vector2i>& positions,
        size_t range,
        const Vector2i& origin)
    {
        return filterInRange(positions, 0, range, origin);
    }

    std::vector<Vector2i> filterInRange(
        TileMap& tileMap,
        size_t rangeStart,
        size_t rangeEnd,
        const Vector2i& origin)
    {
        return filterInRange(filterAllPositions(tileMap), rangeStart, rangeEnd, origin);
    }

    std::vector<Vector2i> filterInRange(
        TileMap& tileMap,
        size_t range,
        const Vector2i& origin)
    {
        return filterInRange(tileMap, 0, range, origin);
    }

    bool isInRange(
        const Vector2i& target,
        size_t rangeStart,
        size_t rangeEnd,
        const Vector2i& origin,
        TileMap& tileMap)
    {
        return isInPositions(target, filterInRange(tileMap, rangeStart, rangeEnd, origin));
    }
    bool isInRange(
        const Vector2i& target,
        size_t range,
        const Vector2i& origin,
        TileMap& tileMap)
    {
        return isInRange(target, 0, range, origin, tileMap);
    }

    RangeSeparatedPositions filterReachable(
        const std::vector<Vector2i>& positions,
        size_t range,
        const Vector2i& origin)
    {
        RangeSeparatedPositions steppedPositions{};

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
            if (!isInPositions(nextTilePosition, positions))
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
                for (auto R : {M_ROTATE_NONE, M_ROTATE_CCW, M_ROTATE_CW})
                {
                    // Set next stepped tile position
                    Vector2i nextTilePosition{Vector2Add(steppedTile.tilePosition, Vector2Transform(R, steppedTile.directionAccessed))};

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
                    if (!isInPositions(nextTilePosition, positions))
                        continue;

                    // Add passable tile to stepped tiles
                    steppedPositions[stepLevel].push_back(SteppedPosition(nextTilePosition, Vector2Transform(R, steppedTile.directionAccessed)));
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

    RangeSeparatedPositions filterReachable(
        TileMap& tileMap,
        size_t range,
        const Vector2i& origin)
    {
        return filterReachable(filterInRange(filterNonSolidPositions(tileMap), range, origin), range, origin);
    }

    bool isReachable(
        const Vector2i& target,
        size_t range,
        const Vector2i& origin,
        TileMap& tileMap)
    {
        return isInSteppedTiles(target, filterReachable(tileMap, range, origin));
    }
}