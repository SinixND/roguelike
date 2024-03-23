#include "UnitMovement.h"

#include "Constants.h"
#include "Movement.h"
#include "Pathfinder.h"
#include "TileTransformation.h"
#include "Transformation.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

namespace UnitMovement
{
    void setTarget(
      World& gameWorld,
      Unit& unit,
      Transformation& cursorPosition)
    {

        // Check if target is valid
        if (
          gameWorld.mapOverlay().contains(cursorPosition.tilePosition()) &&
          !Vector2Equals(
            cursorPosition.tilePosition(),
            unit.transform.tilePosition())) {

            // Set movement target
            unit.movement.setTarget(cursorPosition.tilePosition());
            unit.movement.setIsTargetSet(true);
        }
    }

    void triggerMovement(Movement& movement, Path& path, bool& isInputBlocked)
    {
        if (movement.isTargetSet() && !movement.isMoving()) {

            // Reset (doesnt interrupt movement)
            movement.setIsTargetSet(false);

            // Set path
            movement.setMovePath(path);

            // Trigger movment
            movement.setIsMoving(true);

            // Block user input while moving
            isInputBlocked = true;
        }
    }

    void processMovement(Unit& unit, bool& isInputBlocked)
    {
        bool isTargetReached{false};

        if (unit.movement.isMoving()) {

            // Move unit
            isTargetReached = moveAlongPath(unit, GetFrameTime());
        }

        // Allow user input once target reached
        if (isTargetReached) {
            unit.setIsSelected(false);
            isInputBlocked = false;
        }
    }

    bool moveAlongPath(Unit& unit, float dt)
    {

        // Fraction of one tile size for path progressing
        static float tileFraction{};

        // Distance moved this frame
        float frameDistance = (unit.movement.speed() * TILE_SIZE) * dt;

        // Update distances
        tileFraction += frameDistance;

        // Move
        // Update position: move in direction provided by path
        unit.transform.setPosition(Vector2Add(
          unit.transform.position(),
          Vector2Scale(
            unit.movement.movepath().back().directionAccessed,
            frameDistance)));

        // Progress path
        if (tileFraction > TILE_SIZE) {
            tileFraction = 0;

            unit.transform = TileTransformation::positionToWorld(
              unit.movement.movepath().back().tile->transform.tilePosition());
            unit.movement.popFromMovepath();
        }

        // Check if target reached
        if (unit.movement.movepath().empty()) {
            unit.movement.setIsMoving(false);
            return true;
        }

        return false;
    }

}
