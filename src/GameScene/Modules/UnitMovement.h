#ifndef IG20240308012447
#define IG20240308012447

#include "GamePhase.h"
#include "Movement.h"
#include "Pathfinder.h"
#include "Position.h"
#include "Unit.h"
#include "World.h"
#include <raylib.h>

namespace UnitMovement
{
    void setTarget(World& gameWorld, Unit& unit, PositionComponent& cursorPosition);
    void triggerMovement(MovementComponent& movementComponent, Path& path, bool& isInputBlocked);
    void processMovement(Unit& unit, bool& isInputBlocked, GamePhase& gamePhase);
}

#endif