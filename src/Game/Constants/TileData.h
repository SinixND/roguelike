#ifndef IG20240531224246
#define IG20240531224246

#include <raylib.h>

namespace TileData
{
    //* Odd number to have dedicated center
    float constexpr TILE_SIZE{32};
    Vector2 constexpr TILE_CENTER{0.5f * TILE_SIZE, 0.5f * TILE_SIZE};
    float constexpr TILE_SIZE_HALF{0.5f * TILE_SIZE};
    Vector2 constexpr TILE_DIMENSIONS{TILE_SIZE, TILE_SIZE};
}

#endif
