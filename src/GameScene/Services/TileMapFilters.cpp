#include "TileMapFilters.h"

#include "Constants.h"
#include "SparseSet.h"
#include "Tile.h"
#include "TileMap.h"
#include <cstddef>
#include <raylibEx.h>
#include <vector>

namespace TileMapFilters
{
    bool isInTiles(
        Vector2I target,
        std::vector<Tile*> const& tiles)
    {
        for (auto tile : tiles)
        {
            if (Vector2Equals(
                    target,
                    tile->transform.tilePosition()))
            {
                return true;
            }
        }
        return false;
    }

    bool isInSteppedTiles(
        Vector2I target,
        std::vector<SteppedTile> const& steppedTiles)
    {
        for (auto const& steppedTile : steppedTiles)
        {
            if (Vector2Equals(target, steppedTile.tile->transform.tilePosition()))
            {
                return true;
            }
        }
        return false;
    }

    bool isInRangeSeparatedTiles(
        Vector2I target,
        RangeSeparatedTiles const& sortedSteppedTiles)
    {
        for (auto const& steppedTiles : sortedSteppedTiles)
        {
            for (auto const& steppedTile : steppedTiles)
            {
                if (Vector2Equals(
                        target,
                        steppedTile.tile->transform.tilePosition()))
                {
                    return true;
                }
            }
        }
        return false;
    }

    // Returns all positions
    std::vector<Tile*> filterAllPositions(TileMap& tileMap)
    {
        std::vector<Tile*> tiles{};

        for (auto& tile : tileMap.values())
        {
            tiles.push_back(&tile);
        }

        return tiles;
    }

    // Returns accessible positions (non-solid tiles)
    std::vector<Tile*> filterNonSolidTiles(TileMap& tileMap)
    {
        std::vector<Tile*> accessibleTiles{};

        for (auto& tile : tileMap.values())
        {
            if (tile.isSolid())
            {
                continue;
            }

            accessibleTiles.push_back(&tile);
        }

        return accessibleTiles;
    }

    // Returns inaccessible positions (solid tiles)
    std::vector<Tile*> filterSolidTiles(TileMap& tileMap)
    {
        std::vector<Tile*> tiles{};

        for (auto& tile : tileMap.values())
        {
            if (!tile.isSolid())
            {
                continue;
            }
            tiles.push_back(&tile);
        }

        return tiles;
    }

    // Returns opaque positions (non vision blocking tiles)
    std::vector<Tile*> filterNonOpaqueTiles(TileMap& tileMap)
    {
        std::vector<Tile*> tiles{};

        for (auto& tile : tileMap.values())
        {
            if (tile.blocksVision())
            {
                continue;
            }
            tiles.push_back(&tile);
        }

        return tiles;
    }

    // Returns accessible positions (vision blocking tiles)
    std::vector<Tile*> filterOpaqueTiles(TileMap& tileMap)
    {
        std::vector<Tile*> tiles{};

        for (auto& tile : tileMap.values())
        {
            if (!tile.blocksVision())
            {
                continue;
            }
            tiles.push_back(&tile);
        }

        return tiles;
    }

    std::vector<Tile*> filterInRange(
        std::vector<Tile*> const& tiles,
        int rangeStart,
        int rangeEnd,
        Vector2I origin)
    {
        std::vector<Tile*> inRangeTiles{};

        // Check if ranges are valid
        if (rangeEnd == 0 || rangeEnd < rangeStart)
        {
            return inRangeTiles;
        }

        for (auto* tile : tiles)
        {
            Vector2I delta{
                Vector2Subtract(
                    tile->transform.tilePosition(),
                    origin)};

            int sumDelta{Vector2Sum(delta)};

            if (sumDelta < rangeStart || sumDelta > rangeEnd)
            {
                continue;
            }

            inRangeTiles.push_back(tile);
        }

        return inRangeTiles;
    }

    std::vector<Tile*> filterInRange(
        std::vector<Tile*> const& tiles,
        int range,
        Vector2I origin)
    {
        return filterInRange(tiles, 0, range, origin);
    }

    std::vector<Tile*> filterInRange(
        TileMap& tileMap,
        int rangeStart,
        int rangeEnd,
        Vector2I origin)
    {
        return filterInRange(filterAllPositions(tileMap), rangeStart, rangeEnd, origin);
    }

    std::vector<Tile*> filterInRange(
        TileMap& tileMap,
        int range,
        Vector2I origin)
    {
        return filterInRange(tileMap, 0, range, origin);
    }

    bool isInRange(
        Vector2I target,
        int rangeStart,
        int rangeEnd,
        Vector2I origin,
        TileMap& tileMap)
    {
        return isInTiles(target, filterInRange(tileMap, rangeStart, rangeEnd, origin));
    }

    bool isInRange(
        Vector2I target,
        int range,
        Vector2I origin,
        TileMap& tileMap)
    {
        return isInRange(target, 0, range, origin, tileMap);
    }

    RangeSeparatedTiles filterMovable(
        std::vector<Tile*> const& inRangeMapTiles,
        int moveRange,
        Vector2I origin)
    {
        snd::SparseSet<Vector2I, Tile*> tileSet{};

        for (auto& tile : inRangeMapTiles)
        {
            if (!tile->isSolid())
            {
                tileSet.createOrUpdate(tile->transform.tilePosition(), tile);
            }
        }

        RangeSeparatedTiles sortedSteppedTiles{};

        // Check if range is 0
        if (!moveRange)
        {
            return sortedSteppedTiles;
        }

        // Step 0

        // Extend vector by one additional step level
        sortedSteppedTiles.push_back(std::vector<SteppedTile>());

        // Add starting position
        sortedSteppedTiles.front().push_back(SteppedTile(*tileSet.at(origin)));

        // Step 1

        // Extend vector by one additional step level
        sortedSteppedTiles.push_back(std::vector<SteppedTile>());

        // Test all four directions for first step
        for (Vector2I direction : {
                 V_RIGHT,
                 V_DOWN,
                 V_LEFT,
                 V_UP,
             })
        {
            // Set next stepped tile position
            auto nextTilePosition{Vector2Add(origin, direction)};

            // Check if next tile is in range
            if (!isInTiles(nextTilePosition, inRangeMapTiles))
            {
                continue;
            }

            // Add stepped tile to stepped tiles
            sortedSteppedTiles.back().push_back(SteppedTile(*tileSet.at(nextTilePosition), direction));
        }

        // Steps > 1

        for (int stepLevel{2}; stepLevel <= moveRange; ++stepLevel)
        {
            // Extend vector by one additional step level
            sortedSteppedTiles.push_back(std::vector<SteppedTile>());

            int previousStepLevel{stepLevel - 1};
            size_t previousTilesCount{sortedSteppedTiles[previousStepLevel].size()};

            // Iterate previous stepped tiles to step one further from
            for (size_t tileIndex{0}; tileIndex < previousTilesCount; ++tileIndex)
            {
                SteppedTile lastSteppedTile{sortedSteppedTiles[previousStepLevel][tileIndex]};

                // Check the 3 neighbours it was not stepped from
                for (auto R : {M_ROTATE_NONE, M_ROTATE_CCW, M_ROTATE_CW})
                {
                    // Set next stepped tile position
                    Vector2I nextTilePosition{Vector2Add(lastSteppedTile.tile->transform.tilePosition(), Vector2Transform(R, lastSteppedTile.directionAccessed))};

                    // Check if tile is already known
                    bool tileKnown{false};

                    for (auto const& steppedTiles : sortedSteppedTiles)
                    {
                        for (auto const& steppedTile : steppedTiles)
                        {
                            if (!Vector2Equals(steppedTile.tile->transform.tilePosition(), nextTilePosition))
                            {
                                continue;
                            }

                            tileKnown = true;
                            break;
                        }
                    }

                    if (tileKnown)
                    {
                        continue;
                    }

                    // Check if next tile is in range
                    if (!isInTiles(nextTilePosition, inRangeMapTiles))
                    {
                        continue;
                    }

                    // Add passable tile to stepped tiles
                    sortedSteppedTiles[stepLevel].push_back(
                        SteppedTile(
                            *tileSet.at(
                                nextTilePosition),
                            Vector2Transform(
                                R,
                                lastSteppedTile.directionAccessed)));
                }
            }

            if (sortedSteppedTiles[stepLevel].empty())
            {
                sortedSteppedTiles.pop_back();
                break;
            }
        }

        return sortedSteppedTiles;
    }

    RangeSeparatedTiles filterMovable(
        TileMap& tileMap,
        int moveRange,
        Vector2I origin)
    {
        return filterMovable(
            filterInRange(
                filterNonSolidTiles(tileMap),
                moveRange,
                origin),
            moveRange,
            origin);
    }

    bool isMovable(
        Vector2I target,
        int moveRange,
        Vector2I origin,
        TileMap& tileMap)
    {
        return isInRangeSeparatedTiles(target, filterMovable(tileMap, moveRange, origin));
    }

    std::vector<Tile*> filterEdgeTiles(RangeSeparatedTiles const& tiles)
    {
        std::vector<Tile*> edgeTiles{};

        // check provided tiles
        for (auto& steppedTiles : tiles)
        {
            for (auto& steppedTile : steppedTiles)
            {
                Tile* tile{steppedTile.tile};

                // check neighbours
                for (auto dir : {V_LEFT, V_RIGHT, V_UP, V_DOWN})
                {
                    // Skip if neighbour tile is in tiles
                    Vector2I position{
                        Vector2Add(
                            tile->transform.tilePosition(),
                            dir)};

                    if (isInRangeSeparatedTiles(
                            position,
                            tiles))
                    {
                        continue;
                    }

                    edgeTiles.push_back(tile);
                    break;
                }
            }
        }

        return edgeTiles;
    }

    std::vector<Tile*> filterInActionRange(
        TileMap& tileMap,
        int actionRange,
        int moveRange,
        Vector2I origin)
    {
        std::vector<Tile*> inActionRangeTiles{};

        RangeSeparatedTiles movableTiles{
            filterMovable(
                tileMap,
                moveRange,
                origin)};

        std::vector<Tile*> edgeTiles{filterEdgeTiles(movableTiles)};

        for (auto& edgeTile : edgeTiles)
        {
            auto inRangeTiles{
                filterInRange(
                    tileMap,
                    actionRange,
                    edgeTile->transform.tilePosition())};

            for (auto& inRangeTile : inRangeTiles)
            {
                if (isInRangeSeparatedTiles(inRangeTile->transform.tilePosition(), movableTiles))
                {
                    continue;
                }

                inActionRangeTiles.push_back(inRangeTile);
            }
        }

        return inActionRangeTiles;
    }
}