#ifndef IG20240128183417
#define IG20240128183417

#include "Constants.h"
#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <cstddef>
#include <raylibEx.h>
#include <vector>

struct SteppedTile
{
    Tile* tile;
    Vector2i directionAccessed; // in which it was accessed

    SteppedTile(Tile* tile = nullptr, Vector2i directionAccessed = V_NODIR)
        : tile(tile)
        , directionAccessed(directionAccessed)
    {
    }
};

using RangeSeparatedTiles = std::vector<std::vector<SteppedTile>>;

namespace TileMapFilters
{
    bool isInTiles(
        const Vector2i& target,
        const std::vector<Tile*>& tiles);

    bool isInSteppedTiles(
        const Vector2i& target,
        const std::vector<SteppedTile>& steppedTiles);

    bool isInRangeSeparatedTiles(
        const Vector2i& target,
        const RangeSeparatedTiles& rangeSeparatedTiles);

    std::vector<Tile*> filterNonSolidTiles(TileMap& tileMap);

    std::vector<Tile*> filterInRange(
        const std::vector<Tile*>& tiles,
        size_t rangeStart,
        size_t rangeEnd,
        const Vector2i& origin);

    std::vector<Tile*> filterInRange(
        const std::vector<Tile*>& tiles,
        size_t range,
        const Vector2i& origin);

    std::vector<Tile*> filterInRange(
        TileMap& tileMap,
        size_t rangeStart,
        size_t rangeEnd,
        const Vector2i& origin);

    std::vector<Tile*> filterInRange(
        TileMap& tileMap,
        size_t range,
        const Vector2i& origin);

    bool isInRange(
        const Vector2i& target,
        size_t rangeStart,
        size_t rangeEnd,
        const Vector2i& origin,
        TileMap& tileMap);

    bool isInRange(
        const Vector2i& target,
        size_t range,
        const Vector2i& origin,
        TileMap& tileMap);

    RangeSeparatedTiles filterReachable(
        const std::vector<Tile*>& inRangeTiles,
        size_t range,
        const Vector2i& origin);

    RangeSeparatedTiles filterReachable(
        TileMap& tileMap,
        size_t range,
        const Vector2i& origin);

    bool isReachable(
        const Vector2i& target,
        size_t range,
        const Vector2i& origin,
        TileMap& tileMap);
}

#endif
