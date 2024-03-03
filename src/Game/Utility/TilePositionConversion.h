#ifndef _20240215012142
#define _20240215012142

#include "Constants.h"
#include "RuntimeDatabase.h"
#include "TilePosition.h"
#include "raylib.h"
#include "raylibEx.h"

inline Vector2 tilePositionToScreen(const Vector2i& position)
{
    Vector2 worldPixel{
        position.x * TILE_SIZE,
        position.y * TILE_SIZE};

    return GetWorldToScreen2D(worldPixel, dtb::camera());
}

inline Vector2i screenToTilePosition(const Vector2& pixel)
{
    Vector2 worldPixel{GetScreenToWorld2D(pixel, dtb::camera())};

    return Vector2i{
        static_cast<int>(std::floor((worldPixel.x + (TILE_SIZE / 2)) / TILE_SIZE)),
        static_cast<int>(std::floor((worldPixel.y + (TILE_SIZE / 2)) / TILE_SIZE))};
}

inline Vector2 tilePositionToWorld(const Vector2i& position)
{
    return Vector2{
        (position.x * TILE_SIZE),
        (position.y * TILE_SIZE)};
}

inline Vector2i worldToTilePosition(const Vector2& pixel)
{
    return Vector2i{
        static_cast<int>((pixel.x / TILE_SIZE)),
        static_cast<int>((pixel.y / TILE_SIZE))};
}

inline TilePosition getMouseTile()
{
    return screenToTilePosition(GetMousePosition());
}

#endif