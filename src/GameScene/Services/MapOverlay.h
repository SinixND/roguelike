#ifndef IG20240308011247
#define IG20240308011247

#include "Pathfinder.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"
#include <cstddef>

namespace MapOverlay
{
    void showUnitMoveRange(Unit& unit, World& world);
    void showUnitActionRange(Unit& unit, World& world);
    Path& showPath(Vector2i unitPosition, Vector2i cursorPosition, size_t unitRange, World& world, bool& isPathShown);
}

#endif