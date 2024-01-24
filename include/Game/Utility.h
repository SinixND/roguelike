#ifndef UTILITY_H_20240106173048
#define UTILITY_H_20240106173048

#include "ECS.h"
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include <vector>

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

const Vector2 V_NODIR{0, 0};
const Vector2 V_LEFT{-1, 0};
const Vector2 V_RIGHT{1, 0};
const Vector2 V_UP{0, -1};
const Vector2 V_DOWN{0, 1};

const Matrix2x2 M_ROTATE_NONE{1, 0, 0, 1};
const Matrix2x2 M_ROTATE_RIGHT{0, -1, 1, 0};
const Matrix2x2 M_ROTATE_LEFT{0, 1, -1, 0};

const Vector2 convertToTile(const Vector2& pixelCoordinates);
const Vector2 convertToPixel(const Vector2& tileCoordinates);

// X is right-positive, Y is down-positive
inline Vector2 Vector2MatrixMultiply(const Matrix2x2& M, const Vector2& V);

std::vector<Vector2> findPath(snd::ECS* ecs, Vector2& from, Vector2& target, size_t range);

#endif
