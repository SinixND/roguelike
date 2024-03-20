#ifndef IG20240307161500
#define IG20240307161500

#include "TileMap.h"
#include "TileMapFilters.h"
#include <cstddef>
#include <raylibEx.h>
#include <vector>

using Path = std::vector<SteppedTile>;

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
Path findPath(
    RangeSeparatedTiles const& reachableTiles,
    Vector2i target);

Path findPath(
    TileMap& tileMap,
    Vector2i origin,
    Vector2i target,
    size_t range);

#endif
