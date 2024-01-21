#ifndef UTILITY_H_20240106173048
#define UTILITY_H_20240106173048

#include <cstddef>
#include <raylib.h>
#include <raymath.h>

typedef enum
{
    NODIR = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN,
} Direction;

const Vector2 VNODIR{0, 0};
const Vector2 VLEFT{-1, 0};
const Vector2 VRIGHT{1, 0};
const Vector2 VUP{0, -1};
const Vector2 VDOWN{0, 1};

const Vector2 convertToTile(const Vector2& pixelCoordinates);
const Vector2 convertToPixel(const Vector2& tileCoordinates);
bool isInReach(const Vector2& target, const Vector2& from, size_t range = 0);

#endif
