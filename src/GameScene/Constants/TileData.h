#ifndef IG20240531224246
#define IG20240531224246

#include <raylib.h>

namespace TileData
{
    // Odd number to have dedicated center
    float constexpr TILE_SIZE{32};
    Vector2 constexpr TILE_CENTER{TILE_SIZE / 2, TILE_SIZE / 2};
    float constexpr TILE_SIZE_HALF{TILE_SIZE / 2};
    Vector2 constexpr TILE_DIMENSIONS{TILE_SIZE, TILE_SIZE};
}

#endif