#ifndef UTILITY_H_20240106173048
#define UTILITY_H_20240106173048

#include "ECS.h"
#include "raylibEx.h"
#include <cstddef>
#include <vector>

typedef enum
{
    NODIR = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN,
} Direction;

typedef enum
{
    FLOOR,
    WALL,
} TileType;

// X is right-positive, Y is down-positive
const Vector2Int V_NODIR{0, 0};
const Vector2Int V_LEFT{-1, 0};
const Vector2Int V_RIGHT{1, 0};
const Vector2Int V_UP{0, -1};
const Vector2Int V_DOWN{0, 1};

const Matrix2x2Int M_ROTATE_NONE{1, 0, 0, 1};
const Matrix2x2Int M_ROTATE_RIGHT{0, -1, 1, 0};
const Matrix2x2Int M_ROTATE_LEFT{0, 1, -1, 0};

Vector2Int convertToTile(const Vector2& pixelCoordinates);
Vector2 convertToPixel(const Vector2Int& tileCoordinates);

#endif
