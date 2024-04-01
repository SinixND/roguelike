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

// Render area
float const LEFT_MAP_RENDER_OFFSET{0};
float const TOP_MAP_RENDER_OFFSET{2.0f * TILE_SIZE};
float const RIGHT_MAP_RENDER_OFFSET{0};
float const BOTTOM_MAP_RENDER_OFFSET{100};

float const MAP_RENDER_AREA_MARGIN{0.5f * TILE_SIZE};
//=====================================

#endif