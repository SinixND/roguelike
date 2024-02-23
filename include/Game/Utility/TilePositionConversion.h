#ifndef _20240215012142
#define _20240215012142

#include "Constants.h"
#include "RuntimeDatabase.h"
#include "raylib.h"
#include "raylibEx.h"

inline Vector2 positionToScreen(const Vector2Int& position)
{
    Vector2 worldPixel{
        position.x * TILE_SIZE,
        position.y * TILE_SIZE};

    return GetWorldToScreen2D(worldPixel, dtb::camera());
}

inline Vector2Int screenToPosition(const Vector2& pixel)
{
    Vector2 worldPixel{GetScreenToWorld2D(pixel, dtb::camera())};

    return Vector2Int{
        static_cast<int>(std::floor((worldPixel.x + (TILE_SIZE / 2)) / TILE_SIZE)),
        static_cast<int>(std::floor((worldPixel.y + (TILE_SIZE / 2)) / TILE_SIZE))};
}

inline Vector2 positionToWorld(const Vector2Int& position)
{
    return Vector2{
        (position.x * TILE_SIZE),
        (position.y * TILE_SIZE)};
}

inline Vector2Int worldToPosition(const Vector2& pixel)
{
    return Vector2Int{
        static_cast<int>((pixel.x / TILE_SIZE)),
        static_cast<int>((pixel.y / TILE_SIZE))};
}

inline void setMouseTile(Vector2Int& position)
{
    Vector2Int newPosition{
        screenToPosition(GetMousePosition())};

    position.x = newPosition.x;
    position.y = newPosition.y;
}

#endif
