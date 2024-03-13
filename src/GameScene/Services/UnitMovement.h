#ifndef IG20240308012447
#define IG20240308012447

#include "Movement.h"
#include "Pathfinder.h"
#include "Position.h"
#include "Unit.h"
#include "World.h"
#include <raylib.h>

namespace UnitMovement
{
    void setTarget(World& gameWorld, Unit& unit, Position& cursorPosition);
    void triggerMovement(Movement& movement, Path& path, bool& inputBlocked);
    void processMovement(Unit& unit, bool& inputBlocked);
}

#endif
