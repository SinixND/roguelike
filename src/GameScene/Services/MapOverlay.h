#ifndef IG20240308011247
#define IG20240308011247

#include "Pathfinder.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"

namespace MapOverlay
{
    void showUnitMoveRange(Unit& unit, World& world);
    void showUnitActionRange(Unit& unit, World& world);
    Path& showPath(Vector2I unitPosition, Vector2I cursorPosition, int unitRange, World& world);
}

#endif