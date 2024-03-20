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
        Vector2i target,
        std::vector<Tile*> const& tiles);

    bool isInSteppedTiles(
        Vector2i target,
        std::vector<SteppedTile> const& steppedTiles);

    bool isInRangeSeparatedTiles(
        Vector2i target,
        RangeSeparatedTiles const& rangeSeparatedTiles);

    std::vector<Tile*> filterNonSolidTiles(TileMap& tileMap);

    std::vector<Tile*> filterInRange(
        std::vector<Tile*> const& tiles,
        size_t rangeStart,
        size_t rangeEnd,
        Vector2i origin);

    std::vector<Tile*> filterInRange(
        std::vector<Tile*> const& tiles,
        size_t range,
        Vector2i origin);

    std::vector<Tile*> filterInRange(
        TileMap& tileMap,
        size_t rangeStart,
        size_t rangeEnd,
        Vector2i origin);

    std::vector<Tile*> filterInRange(
        TileMap& tileMap,
        size_t range,
        Vector2i origin);

    bool isInRange(
        Vector2i target,
        size_t rangeStart,
        size_t rangeEnd,
        Vector2i origin,
        TileMap& tileMap);

    bool isInRange(
        Vector2i target,
        size_t range,
        Vector2i origin,
        TileMap& tileMap);

    RangeSeparatedTiles filterMovable(
        std::vector<Tile*> const& inRangeTiles,
        size_t moveRange,
        Vector2i origin);

    RangeSeparatedTiles filterMovable(
        TileMap& tileMap,
        size_t moveRange,
        Vector2i origin);

    bool isMovable(
        Vector2i target,
        size_t moveRange,
        Vector2i origin,
        TileMap& tileMap);

    std::vector<Tile*> filterEdgeTiles(RangeSeparatedTiles const& tiles);

    std::vector<Tile*> filterInActionRange(
        TileMap& tileMap,
        size_t actionRange,
        size_t moveRange,
        Vector2i origin);
}

#endif
