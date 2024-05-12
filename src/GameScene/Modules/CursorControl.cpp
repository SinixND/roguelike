#include "CursorControl.h"

#include "CameraControl.h"
#include "Directions.h"
#include "Panels.h"
#include "Position.h"
#include "TileTransformation.h"
#include "Timer.h"
#include "raylibEx.h"
#include <raylib.h>

namespace CursorControl
{
    auto isOutOfRectangle(Vector2I position, Rectangle rectangle, Camera2D const& camera) -> bool
    {
        return !CheckCollisionPointRec(
            TileTransformation::positionToScreen(position, camera),
            rectangle);
    }

    auto getNewCursorPosition(Vector2I cursorPosition, Vector2I direction, int boostFactor) -> Vector2I
    {
        return Vector2I{
            cursorPosition.x + (direction.x * boostFactor),
            cursorPosition.y + (direction.y * boostFactor)};
    }

    auto cursorWouldGoOutOfScreen(Vector2I& newCursorPosition, PositionComponent const& cursorPosition, Vector2I const& dir, int& factor, Camera2D const& camera) -> bool
    {
        // If new position were out of screen with potential boost applied
        Rectangle renderRectangle{PanelMap::setup().rectangle()};

        if (isOutOfRectangle(
                newCursorPosition,
                renderRectangle,
                camera))
        {
            // Return early if no boost applied
            if (factor == 1)
            {
                return true;
            }

            // Check again without boost
            factor = 1;

            // Remove boost from cursor position
            newCursorPosition = getNewCursorPosition(
                cursorPosition.tilePosition(),
                dir,
                factor);

            // If still out of render rectangle
            if (isOutOfRectangle(
                    newCursorPosition,
                    renderRectangle,
                    camera))
            {
                return true;
            }
        }

        return false;
    }

    void setDirection(Vector2I& dir, int const& key, int const& keyPressed, snx::Timer& timer, snx::Timer& delay)
    {
        switch (key)
        {
        case KEY_W:
        case KEY_UP:
        {
            if (keyPressed || (delay.hasDelayPassed(IsKeyDown(key)) && timer.hasTimePassed()))
            {
                dir = Directions::V_UP;
            }
        }
        break;

        case KEY_A:
        case KEY_LEFT:
        {
            if (keyPressed || (delay.hasDelayPassed(IsKeyDown(key)) && timer.hasTimePassed()))
            {
                dir = Directions::V_LEFT;
            }
        }
        break;

        case KEY_S:
        case KEY_DOWN:
        {
            if (keyPressed || (delay.hasDelayPassed(IsKeyDown(key)) && timer.hasTimePassed()))
            {
                dir = Directions::V_DOWN;
            }
        }
        break;

        case KEY_D:
        case KEY_RIGHT:
        {
            if (keyPressed || (delay.hasDelayPassed(IsKeyDown(key)) && timer.hasTimePassed()))
            {
                dir = Directions::V_RIGHT;
            }
        }
        break;

        default:
            break;
        }
    }

    void processKeyControl(PositionComponent& cursorPosition, Camera2D const& camera)
    {
        // Store last key
        static int key{};
        int keyPressed{GetKeyPressed()};

        if (keyPressed)
        {
            key = keyPressed;
        }

        Vector2I dir{};

        static snx::Timer timer{CURSOR_MOVE_TICK};
        static snx::Timer delay{CURSOR_MOVE_DELAY};

        // Set direction
        setDirection(dir, key, keyPressed, timer, delay);

        // No boost by default
        int factor{1};

        // Apply pan boost factor to cursor movement when shift is pressed
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            factor = CameraControl::PAN_BOOST_FACTOR;
        }

        Vector2I newCursorPosition{
            getNewCursorPosition(
                cursorPosition.tilePosition(),
                dir,
                factor)};

        // Check if cursor would go out of screen, removes boost if helpful
        if (cursorWouldGoOutOfScreen(newCursorPosition, cursorPosition, dir, factor, camera))
        {
            return;
        }

        // Set verified position
        cursorPosition.setTilePosition(newCursorPosition);
    }

    void update(PositionComponent& cursorPosition, Camera2D const& camera, bool isMouseControlled)
    {
        // Cursor control
        if (isMouseControlled)
        {
            // Mouse controlled cursor
            cursorPosition.setTilePosition(TileTransformation::getMouseTile(camera));
        }
        else
        {
            processKeyControl(cursorPosition, camera);
        }
    }
}
