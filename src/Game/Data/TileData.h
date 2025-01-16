#ifndef IG20240531224246
#define IG20240531224246

#include <raylib.h>

namespace TileData
{
    //* Odd number to have dedicated center
    float constexpr tileSize{ 32 };
    Vector2 constexpr tileCenter{ 0.5f * tileSize, 0.5f * tileSize };
    float constexpr tileSizeHalf{ 0.5f * tileSize };
    Vector2 constexpr tileDimensions{ tileSize, tileSize };
}

#endif
