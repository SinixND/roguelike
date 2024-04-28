#ifndef IG20240307180713
#define IG20240307180713

#include "Position.h"
#include <raylib.h>

namespace CursorControl
{
    float constexpr CURSOR_MOVE_TICK{0.1F};   // time in seconds to next cursor move step
    float constexpr CURSOR_MOVE_DELAY{0.25F}; // time in seconds to next cursor move step

    void update(PositionComponent& cursorPosition, Camera2D const& camera, bool isMouseControlled);
}

#endif