#include "UnitMovement.h"

#include "Movement.h"
#include "Pathfinder.h"
#include "Position.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"
#include <raylib.h>

namespace UnitMovement
{
    void setTarget(World& gameWorld, Unit& unit, Position& cursorPosition)
    {
        // Check if target is valid
        if (
            gameWorld.mapOverlay().contains(cursorPosition.tilePosition()) &&
            !Vector2Equals(cursorPosition.tilePosition(), unit.position.tilePosition()))
        {
            // Set movement target
            unit.movement.setTarget(cursorPosition.tilePosition());
            unit.movement.setIsTargetSet(true);
        }
    }

    void triggerMovement(Movement& movement, Path& path, bool& isInputBlocked)
    {
        if (movement.isTargetSet() && !movement.isMoving())
        {
            // Reset (doesnt interrupt movement)
            movement.setIsTargetSet(false);

            // Setting path triggers movment
            movement.setPath(path);

            isInputBlocked = true;
        }
    }

    void processMovement(Unit& unit, bool& isInputBlocked)
    {
        bool isTargetReached{false};
        if (unit.movement.isMoving())
        {
            // Move unit
            isTargetReached = unit.movement.move(
                unit.position.get(),
                GetFrameTime());
        }

        if (isTargetReached)
        {
            unit.isSelected = false;
            isInputBlocked = false;
        }
    }
}
