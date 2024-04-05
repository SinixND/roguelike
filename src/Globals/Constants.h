#ifndef IG20240217235908
#define IG20240217235908

#include "raylibEx.h"
#include <raylib.h>

bool const VSYNC_ACTIVE{false};

// Direction vectors
// X is right-positive, Y is down-positive
Vector2 const V_NULL{0, 0};
Vector2I const V_NODIR{0, 0};
Vector2I const V_LEFT{-1, 0};
Vector2I const V_RIGHT{1, 0};
Vector2I const V_UP{0, -1};
Vector2I const V_DOWN{0, 1};

// Rotation matrices
Matrix2x2I const M_ROTATE_NONE{
    1,
    0,
    0,
    1};

Matrix2x2I const M_ROTATE_CW{
    0,
    -1,
    1,
    0};

Matrix2x2I const M_ROTATE_CCW{
    0,
    1,
    -1,
    0};

Matrix2x2 const M_ROTATE_CW_45{
    0.7071068,
    -0.7071068,
    0.7071068,
    0.7071068};

// Game
//=====================================
// Font
float const FONT_SIZE{20};

// Timings
float const PAN_TICK{0.1f};           // time in seconds to next pan move step
float const CURSOR_MOVE_TICK{0.1f};   // time in seconds to next cursor move step
float const CURSOR_MOVE_DELAY{0.25f}; // time in seconds to next cursor move step

// Textures
float const TEXTURE_WIDTH{32};

// Tile
float const TILE_SIZE{25};
Vector2 const TILE_DIMENSIONS{TILE_SIZE, TILE_SIZE};

// Edge pan
float const EDGE_PAN_FRAME_WIDTH{3.5f * TILE_SIZE};
int const PAN_BOOST_FACTOR{4};

// Zoom (3-step)
float const ZOOM_DEFAULT{1};
float const ZOOM_MIN{0.1f};
float const ZOOM_MAX{3.0f};

// Panels (sorted by descending priority)
float const PANEL_BORDER_WEIGHT{1};
// Info (right); Number: chars per line
float const INFO_PANEL_WIDTH{(10 /*Number of chars per line*/ * FONT_SIZE) + FONT_SIZE};
// Status (top); Number: lines to print
float const STATUS_PANEL_HEIGHT{(1 /*Number of lines to print*/ * (1.5f * FONT_SIZE)) + (0.5f * FONT_SIZE)};
// Log (bottom);
float const LOG_PANEL_HEIGHT{(3 /*Number of lines to print*/ * (1.5f * FONT_SIZE)) + (0.5f * FONT_SIZE)};

// Ray casting
float const RAY_CAST_PRECISION{0.0001f};
//=====================================

#endif