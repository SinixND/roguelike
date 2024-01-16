#ifndef UTILITY_H_20240106173048
#define UTILITY_H_20240106173048

#include <raylib.h>

namespace snd
{
    const Vector2 convertToTile(const Vector2& pixelCoordinates);
    const Vector2 convertToPixel(const Vector2& tileCoordinates);
}

#endif
