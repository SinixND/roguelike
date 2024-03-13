#ifndef IG20240217235908
#define IG20240217235908

#include "raylibEx.h"
#include <raylib.h>

const bool VSYNC_ACTIVE{false};

// Direction vectors
// X is right-positive, Y is down-positive
const Vector2 V_NULL{0, 0};
const Vector2i V_NODIR{0, 0};
const Vector2i V_LEFT{-1, 0};
const Vector2i V_RIGHT{1, 0};
const Vector2i V_UP{0, -1};
const Vector2i V_DOWN{0, 1};

// Rotation matrices
const Matrix2x2i M_ROTATE_NONE{
    1, 0,
    0, 1};

const Matrix2x2i M_ROTATE_CW{
    0, -1,
    1, 0};

const Matrix2x2i M_ROTATE_CCW{
    0, 1,
    -1, 0};

const Matrix2x2 M_ROTATE_CW_45{
    0.7071068, -0.7071068,
    0.7071068, 0.7071068};

// GAME
//=====================================
// Tile
const float TILE_SIZE{25};
const Vector2 TILE_DIMENSIONS{TILE_SIZE, TILE_SIZE};

// Edge pan
const float PAN_SPEED{0.1f}; // time in seconds to next pan progress
const float EDGE_PAN_FRAME_WEIGHT{3.5f * TILE_SIZE};

// Zoom
const float ZOOM_STEP{0.125f};

// Render area
const float LEFT_MAP_RENDER_OFFSET{0};
const float TOP_MAP_RENDER_OFFSET{2.0f * TILE_SIZE};
const float RIGHT_MAP_RENDER_OFFSET{0};
const float BOTTOM_MAP_RENDER_OFFSET{100};

const float MAP_RENDER_AREA_MARGIN{0.5f * TILE_SIZE};
//=====================================

#endif