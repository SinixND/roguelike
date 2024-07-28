#ifndef IG20240215012142
#define IG20240215012142

#include "TileData.h"
#include "raylib.h"
#include "raylibEx.h"
#include <cmath>

namespace UnitConversion
{
    // This file contains functions to convert between
    // - tile position (int/position, relative: considers camera)
    // - world position (float/pixel, relative: considers camera)
    // - screen position (float/pixel, absolute)

    // World pixel to tile position
    inline Vector2I worldToTile(Vector2 pixel)
    {
        return Vector2I{
            static_cast<int>(std::floor((pixel.x + (TileData::TILE_SIZE / 2)) / TileData::TILE_SIZE)),
            static_cast<int>(std::floor((pixel.y + (TileData::TILE_SIZE / 2)) / TileData::TILE_SIZE))};
    }

    // Tile position to world pixel
    inline Vector2 tileToWorld(Vector2I position)
    {
        return Vector2{
            (position.x * TileData::TILE_SIZE),
            (position.y * TileData::TILE_SIZE)};
    }

    // Screen pixel to world pixel to tile position
    inline Vector2I screenToTile(Vector2 pixel, Camera2D const& camera)
    {
        Vector2 worldPixel{GetScreenToWorld2D(pixel, camera)};

        return worldToTile(worldPixel);
    }

    // Tile position to world pixel to screen pixel
    inline Vector2 tileToScreen(Vector2I position, Camera2D const& camera)
    {
        Vector2 worldPixel{
            position.x * TileData::TILE_SIZE,
            position.y * TileData::TILE_SIZE};

        return GetWorldToScreen2D(worldPixel, camera);
    }

    inline Vector2I getMouseTile(Camera2D const& camera)
    {
        return screenToTile(GetMousePosition(), camera);
    }
}

#endif
