#ifndef UTILITY_H_20240106173048
#define UTILITY_H_20240106173048

#include "CONSTANTS.h"
#include "raylib.h"
#include <raymath.h>

namespace snd
{
    const inline Vector2 coordinateToPosition(const Vector2& coordinates)
    {
        Vector2 tileSize{CONSTANTS::getInstance()->getTileSize()};

        return Vector2{static_cast<int>(coordinates.x / tileSize.x) * tileSize.x, static_cast<int>(coordinates.y / tileSize.y) * tileSize.y};
    }
}

#endif