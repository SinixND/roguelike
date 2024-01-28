#ifndef UTILITY_H_20240106173048
#define UTILITY_H_20240106173048

#include "ECS.h"
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include <vector>

template <>
struct std::hash<Vector2>
{
    size_t operator()(const Vector2& V) const noexcept
    {
        size_t h1 = std::hash<double>()(V.x);
        size_t h2 = std::hash<double>()(V.y);
        return (h1 ^ (h2 << 1));
    }
};

inline bool operator==(const Vector2& lhs, const Vector2& rhs)
{
    return Vector2Equals(lhs, rhs);
};

typedef enum
{
    NODIR = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN,
} Direction;

typedef struct
{
    int m11, m12;
    int m21, m22;
} Matrix2x2;

typedef struct
{
    Vector2 position;
    Vector2 dimension;
} Area;

typedef enum
{
    FLOOR_TILE,
    WALL_TILE,
} TileType;

const Vector2 V_NODIR{0, 0};
const Vector2 V_LEFT{-1, 0};
const Vector2 V_RIGHT{1, 0};
const Vector2 V_UP{0, -1};
const Vector2 V_DOWN{0, 1};

const Matrix2x2 M_ROTATE_NONE{1, 0, 0, 1};
const Matrix2x2 M_ROTATE_RIGHT{0, -1, 1, 0};
const Matrix2x2 M_ROTATE_LEFT{0, 1, -1, 0};

Vector2 convertToTile(const Vector2& pixelCoordinates);
Vector2 convertToPixel(const Vector2& tileCoordinates);

// X is right-positive, Y is down-positive
inline Vector2 Vector2MatrixMultiply(const Matrix2x2& M, const Vector2& V)
{
    return Vector2{((M.m11 * V.x) + (M.m12 * V.y)), ((M.m21 * V.x) + (M.m22 * V.y))};
}

inline size_t Vector2LengthTiled(const Vector2& V)
{
    return abs(V.x) + abs(V.y);
}

#endif
