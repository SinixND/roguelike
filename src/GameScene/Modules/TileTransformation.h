#ifndef IG20240215012142
#define IG20240215012142

#include "RuntimeDatabase.h"
#include "Textures.h"
#include "raylib.h"
#include "raylibEx.h"
#include <cmath>

namespace TileTransformation
{
    // World coordinates
    inline Vector2I worldToPosition(Vector2 pixel)
    {
        return Vector2I{
            static_cast<int>(std::floor((pixel.x + (Textures::TILE_SIZE / 2)) / Textures::TILE_SIZE)),
            static_cast<int>(std::floor((pixel.y + (Textures::TILE_SIZE / 2)) / Textures::TILE_SIZE))};
    }

    inline Vector2 positionToWorld(Vector2I position)
    {
        return Vector2{
            (position.x * Textures::TILE_SIZE),
            (position.y * Textures::TILE_SIZE)};
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
            position.x * Textures::TILE_SIZE,
            position.y * Textures::TILE_SIZE};

        return GetWorldToScreen2D(worldPixel, dtb::camera());
    }

    inline Vector2I getMouseTile()
    {
        return screenToPosition(GetMousePosition());
    }
}

#endif
