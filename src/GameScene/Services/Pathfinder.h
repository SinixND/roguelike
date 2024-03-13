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
    const RangeSeparatedTiles& reachableTiles,
    const Vector2i& target);

Path findPath(
    TileMap& tileMap,
    const Vector2i& origin,
    const Vector2i& target,
    size_t range);

#endif
