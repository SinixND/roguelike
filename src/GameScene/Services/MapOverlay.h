#ifndef _20240308011247
#define _20240308011247

#include "Pathfinder.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"
#include <cstddef>

namespace MapOverlay
{
    void showUnitRange(bool& moveRangeShown, Unit& unit, World& world);
    Path& showPath(const Vector2i& unitPosition, const Vector2i& cursorPosition, size_t unitRange, World& world, bool& pathShown);
}

#endif