#ifndef IG20240215012142
#define IG20240215012142

#include "TextureData.h"
#include "raylib.h"
#include "raylibEx.h"
#include <cmath>

namespace TileTransformation
{
    // World coordinates
    inline Vector2I worldToPosition(Vector2 pixel)
    {
        return Vector2I{
            static_cast<int>(std::floor((pixel.x + (TextureData::TILE_SIZE / 2)) / TextureData::TILE_SIZE)),
            static_cast<int>(std::floor((pixel.y + (TextureData::TILE_SIZE / 2)) / TextureData::TILE_SIZE))};
    }

    inline Vector2 positionToWorld(Vector2I position)
    {
        return Vector2{
            (position.x * TextureData::TILE_SIZE),
            (position.y * TextureData::TILE_SIZE)};
    }

    // Screen coordinates
    inline Vector2I screenToPosition(Vector2 pixel, Camera2D const& camera)
    {
        Vector2 worldPixel{GetScreenToWorld2D(pixel, camera)};

        return worldToPosition(worldPixel);
    }

    inline Vector2 positionToScreen(Vector2I position, Camera2D const& camera)
    {
        Vector2 worldPixel{
            position.x * TextureData::TILE_SIZE,
            position.y * TextureData::TILE_SIZE};

        return GetWorldToScreen2D(worldPixel, camera);
    }

    inline Vector2I getMouseTile(Camera2D const& camera)
    {
        return screenToPosition(GetMousePosition(), camera);
    }
}

#endif
