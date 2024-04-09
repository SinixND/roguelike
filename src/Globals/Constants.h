#ifndef IG20240217235908
#define IG20240217235908

#include "raylibEx.h"
#include <raylib.h>

bool constexpr VSYNC_ACTIVE{false};

// Direction vectors
// X is right-positive, Y is down-positive
Vector2 constexpr V_NULL{0, 0};
Vector2I constexpr V_NODIR{0, 0};
Vector2I constexpr V_LEFT{-1, 0};
Vector2I constexpr V_RIGHT{1, 0};
Vector2I constexpr V_UP{0, -1};
Vector2I constexpr V_DOWN{0, 1};

// Rotation matrices
Matrix2x2I constexpr M_ROTATE_NONE{
    1,
    0,
    0,
    1};

Matrix2x2I constexpr M_ROTATE_CW{
    0,
    -1,
    1,
    0};

Matrix2x2I constexpr M_ROTATE_CCW{
    0,
    1,
    -1,
    0};

Matrix2x2 constexpr M_ROTATE_CW_45{
    0.7071068,
    -0.7071068,
    0.7071068,
    0.7071068};

// Game
//=====================================
// Font
float constexpr FONT_SIZE{20};

// Timings
float constexpr PAN_TICK{0.1F};           // time in seconds to next pan move step
float constexpr CURSOR_MOVE_TICK{0.1F};   // time in seconds to next cursor move step
float constexpr CURSOR_MOVE_DELAY{0.25F}; // time in seconds to next cursor move step

// Textures
float constexpr TEXTURE_WIDTH{32};

// Tile
float constexpr TILE_SIZE{25};
Vector2 constexpr TILE_DIMENSIONS{TILE_SIZE, TILE_SIZE};

// Edge pan
float constexpr EDGE_PAN_FRAME_WIDTH{3.5F * TILE_SIZE};
int constexpr PAN_BOOST_FACTOR{4};

// Zoom (3-step)
float constexpr ZOOM_DEFAULT{1};
float constexpr ZOOM_MIN{0.1F};
float constexpr ZOOM_MAX{3.0F};

// Panels (sorted by descending priority)
float constexpr PANEL_BORDER_WEIGHT{1};
// Info (right); Number: chars per line
float constexpr INFO_PANEL_WIDTH{(10 /*Number of chars per line*/ * FONT_SIZE) + FONT_SIZE};
// Status (top); Number: lines to print
float constexpr STATUS_PANEL_HEIGHT{(1 /*Number of lines to print*/ * (1.5F * FONT_SIZE)) + (0.5F * FONT_SIZE)};
// Log (bottom);
float constexpr LOG_PANEL_HEIGHT{(3 /*Number of lines to print*/ * (1.5F * FONT_SIZE)) + (0.5F * FONT_SIZE)};

// Ray casting
float constexpr RAY_CAST_PRECISION{0.0001F};
//=====================================

#endif