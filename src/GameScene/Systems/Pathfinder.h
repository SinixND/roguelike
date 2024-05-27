#ifndef IG20240307161500
#define IG20240307161500

#include "TileMap.h"
#include "TileMapFilters.h"
#include <raylibEx.h>
#include <vector>

using Path = std::vector<SteppedTile>;

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
Path findPath(
    std::vector<std::vector<SteppedTile>> const& reachableTiles,
    Vector2I target);

Path findPath(
    TileMap& tileMap,
    Vector2I origin,
    Vector2I target,
    int range);

#endif
