#ifndef _20240217235908
#define _20240217235908

#include "raylibEx.h"
#include <raylib.h>

const float TILE_SIZE{25};
const Vector2 TILE_DIMENSIONS{TILE_SIZE, TILE_SIZE};
const float DEADZONE_PIXELS{40};
const float ZOOM_STEP{0.125f};

enum DirectionId
{
    NODIR = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

// Direction vectors
// X is right-positive, Y is down-positive
const Vector2 V_NULL{0, 0};
const Vector2Int V_NODIR{0, 0};
const Vector2Int V_LEFT{-1, 0};
const Vector2Int V_RIGHT{1, 0};
const Vector2Int V_UP{0, -1};
const Vector2Int V_DOWN{0, 1};

// Rotation matrices
const Matrix2x2Int M_ROTATE_NONE{
    1, 0,
    0, 1};

const Matrix2x2Int M_ROTATE_RIGHT{
    0, -1,
    1, 0};

const Matrix2x2Int M_ROTATE_LEFT{
    0, 1,
    -1, 0};

#endif