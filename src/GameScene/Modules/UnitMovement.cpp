#include "UnitMovement.h"

#include "GamePhase.h"
#include "Movement.h"
#include "Pathfinder.h"
#include "Position.h"
#include "RenderID.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"
#include <raylib.h>

namespace UnitMovement
{
    void setTarget(World& gameWorld, Unit& unit, PositionComponent& cursorPosition)
    {
        // Check if target is valid
        if (
            gameWorld.mapOverlay().contains(cursorPosition.tilePosition())
            && !Vector2Equals(
                cursorPosition.tilePosition(),
                unit.positionComponent.tilePosition()))
        {
            if (gameWorld.mapOverlay().at(cursorPosition.tilePosition())->graphicComponent.renderID() != RenderID::REACHABLE)
            {
                return;
            }

            // Set movement target
            unit.movementComponent.setTarget(cursorPosition.tilePosition());
        }
    }

    void triggerMovement(MovementComponent& movementComponent, Path& path, bool& isInputBlocked)
    {
        // If target is set but not yet moving
        if (movementComponent.isTargetSet() && !movementComponent.isMoving())
        {
            // Reset (doesnt interrupt movement)
            movementComponent.unsetTarget();

            // Setting path triggers movment
            movementComponent.setPath(path);

            isInputBlocked = true;
        }
    }

    void processMovement(Unit& unit, bool& isInputBlocked, [[maybe_unused]] GamePhase& gamePhase)
    {
        bool isTargetReached{false};
        if (unit.movementComponent.isMoving())
        {
            // Move unit
            isTargetReached = Movement::moveAlongPath(
                unit.positionComponent,
                unit.movementComponent,
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
