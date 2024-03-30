#ifndef IG20240215012142
#define IG20240215012142

#include "Constants.h"
#include "RuntimeDatabase.h"
#include "raylib.h"
#include "raylibEx.h"
#include <cmath>

namespace TileTransformation
{
    // World coordinates
    inline Vector2I worldToPosition(Vector2 pixel)
    {
        return Vector2I{
          static_cast<int>(std::floor((pixel.x + (TILE_SIZE / 2)) / TILE_SIZE)),
          static_cast<int>(std::floor((pixel.y + (TILE_SIZE / 2)) / TILE_SIZE))};
    }

    inline Vector2 positionToWorld(Vector2I position)
    {
        return Vector2{
          (position.x * TILE_SIZE),
          (position.y * TILE_SIZE)};
    }

    // Screen coordinates
    inline Vector2I screenToPosition(Vector2 pixel)
    {
        Vector2 worldPixel{GetScreenToWorld2D(pixel, dtb::camera())};

        return worldToPosition(worldPixel);
    }

    inline Vector2 positionToScreen(Vector2I position)
    {
        Vector2 worldPixel{
          position.x * TILE_SIZE,
          position.y * TILE_SIZE};

        return GetWorldToScreen2D(worldPixel, dtb::camera());
    }

    inline Vector2I getMouseTile()
    {
        return screenToPosition(GetMousePosition());
    }
}

#endif
