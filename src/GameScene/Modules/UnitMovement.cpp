#include "UnitMovement.h"

#include "GamePhase.h"
#include "Movement.h"
#include "Pathfinder.h"
#include "RenderID.h"
#include "Transformation.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"
#include <raylib.h>

namespace UnitMovement
{
    void setTarget(World& gameWorld, Unit& unit, Transformation& cursorPosition)
    {
        // Check if target is valid
        if (
            gameWorld.mapOverlay().contains(cursorPosition.tilePosition())
            && !Vector2Equals(
                cursorPosition.tilePosition(),
                unit.transform.tilePosition()))
        {
            if (gameWorld.mapOverlay().at(cursorPosition.tilePosition())->graphic.renderID() != RenderID::REACHABLE)
            {
                return;
            }

            // Set movement target
            unit.movement.setTarget(cursorPosition.tilePosition());
        }
    }

    void triggerMovement(Movement& movement, Path& path, bool& isInputBlocked)
    {
        // If target is set but not yet moving
        if (movement.isTargetSet() && !movement.isMoving())
        {
            // Reset (doesnt interrupt movement)
            movement.unsetTarget();

            // Setting path triggers movment
            movement.setPath(path);

            isInputBlocked = true;
        }
    }

    void processMovement(Unit& unit, bool& isInputBlocked, [[maybe_unused]] GamePhase& gamePhase)
    {
        bool isTargetReached{false};
        if (unit.movement.isMoving())
        {
            // Move unit
            isTargetReached = unit.movement.move(
                unit.transform,
                GetFrameTime());
        }

        if (isTargetReached)
        {
            //* gamePhase = GamePhase::actionPhase;
            unit.setIsSelected(false);
            isInputBlocked = false;
        }
    }
}
