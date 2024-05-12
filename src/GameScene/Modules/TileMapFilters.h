#ifndef IG20240128183417
#define IG20240128183417

#include "Directions.h"
#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <raylibEx.h>
#include <vector>

struct SteppedTile
{
    Tile* tile;
    Vector2I directionAccessed; // in which it was accessed

    SteppedTile(Tile* tile = nullptr, Vector2I directionAccessed = Directions::V_NODIR)
        : tile(tile)
        , directionAccessed(directionAccessed)
    {
    }
};

using SteppedTiles = std::vector<SteppedTile>;

using RangeSeparatedTiles = std::vector<SteppedTiles>;

namespace TileMapFilters
{
    bool isInTiles(
        Vector2I target,
        std::vector<Tile*> const& tiles);

    bool isInSteppedTiles(
        Vector2I target,
        SteppedTiles const& steppedTiles);

    bool isInRangeSeparatedTiles(
        Vector2I target,
        RangeSeparatedTiles const& rangeSeparatedTiles);

    std::vector<Tile*> filterNonSolidTiles(TileMap& tileMap);

    std::vector<Tile*> filterSolidTiles(TileMap& tileMap);

    std::vector<Tile*> filterNonVisionBlockingTiles(TileMap& tileMap);

    std::vector<Tile*> filterVisionBlockingTiles(TileMap& tileMap);

    std::vector<Tile*> filterInRange(
        std::vector<Tile*> const& tiles,
        int rangeStart,
        int rangeEnd,
        Vector2I origin);

    std::vector<Tile*> filterInRange(
        std::vector<Tile*> const& tiles,
        int range,
        Vector2I origin);

    std::vector<Tile*> filterInRange(
        TileMap& tileMap,
        int rangeStart,
        int rangeEnd,
        Vector2I origin);

    std::vector<Tile*> filterInRange(
        TileMap& tileMap,
        int range,
        Vector2I origin);

    bool isInRange(
        Vector2I target,
        int rangeStart,
        int rangeEnd,
        Vector2I origin,
        TileMap& tileMap);

    bool isInRange(
        Vector2I target,
        int range,
        Vector2I origin,
        TileMap& tileMap);

    RangeSeparatedTiles filterMovableSorted(
        std::vector<Tile*> const& inRangeMapTiles,
        int moveRange,
        Vector2I origin);

    RangeSeparatedTiles filterMovableSorted(
        TileMap& tileMap,
        int moveRange,
        Vector2I origin);

    bool isMovable(
        Vector2I target,
        int moveRange,
        Vector2I origin,
        TileMap& tileMap);

    std::vector<Tile*> filterEdgeTiles(RangeSeparatedTiles const& tiles);

    std::vector<Tile*> filterInActionRange(
        TileMap& tileMap,
        int actionRange,
        int moveRange,
        Vector2I origin);
}

#endif
