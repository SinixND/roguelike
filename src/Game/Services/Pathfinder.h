#ifndef _20240307161500
#define _20240307161500

#include "TileMapFilters.h"
#include "TileMap.h"
#include <raylibEx.h>
#include <vector>

using Path = std::vector<SteppedPosition>;

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
Path findPath(
    const RangeSeparatedPositions& reachableTiles,
    const Vector2i& target);

Path findPath(
    TileMap& tileMap,
    const Vector2i& origin,
    const Vector2i& target,
    size_t range);

#endif
