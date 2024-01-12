#ifndef UTILITY_H_20240106173048
#define UTILITY_H_20240106173048

#include "CONSTANTS.h"
#include "raylib.h"
#include <cmath>
#include <iostream>
#include <string>

namespace snd
{
    const inline Vector2 pixelToTransformedTile(const Vector2& pixelCoordinates)
    {
        static Vector2 screenSize{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};

        if (IsWindowResized())
        {
            screenSize = {static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
        }

        Vector2 tileSize{CONSTANTS::getInstance()->getTileSize()};

        // convert pixel coordinate to transformed tile coordinate (center of screen is tile (0,0))
        return Vector2{
            (std::floor((pixelCoordinates.x - ((screenSize.x / 2) - (tileSize.x / 2))) / tileSize.x)),
            (std::floor((pixelCoordinates.y - ((screenSize.y / 2) - (tileSize.y / 2))) / tileSize.y))};
    }

    const inline Vector2 transformedTileToPixel(const Vector2& tileCoordinates)
    {
        static Vector2 screenSize{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};

        if (IsWindowResized())
        {
            screenSize = {static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
        }

        Vector2 tileSize{CONSTANTS::getInstance()->getTileSize()};

        // convert transformed tile coordinate to pixel coordinate [ tile (0,0) is center of screen]
        return Vector2{
            (tileCoordinates.x * tileSize.x) + (screenSize.x / 2) + (tileSize.x / 2),
            (tileCoordinates.y * tileSize.y) + (screenSize.y / 2) + (tileSize.y / 2)};
    }

}

#endif
