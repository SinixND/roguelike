#ifndef IG20240312000918
#define IG20240312000918

#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

namespace RayCast
{
    float constexpr RAY_CAST_PRECISION{0.0001F};

    // Tiles passed by ray
    auto getTilesPassedByRay(
        std::vector<Vector2I>& rayTargets,
        Vector2 origin,
        TileMap& tileMap) -> std::vector<Tile*>;

    auto getTilesPassedByRay(
        std::vector<Vector2I>& rayTargets,
        Vector2I origin,
        TileMap& tileMap) -> std::vector<Tile*>;
}

#endif