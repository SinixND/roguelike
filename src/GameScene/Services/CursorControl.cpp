#include "CursorControl.h"

#include "Constants.h"
#include "Render.h"
#include "TileTransformation.h"
#include "Timer.h"
#include "Transformation.h"
#include "raylibEx.h"
#include <raylib.h>

namespace CursorControl
{
    namespace
    {
        Vector2I getNewCursorPosition(Vector2I cursorPosition, Vector2I direction, int boostFactor)
        {
            return Vector2I{
              cursorPosition.x + (direction.x * boostFactor),
              cursorPosition.y + (direction.y * boostFactor)};
        }

        bool isOutOfRectangle(Vector2I position, Rectangle rectangle)
        {
            return !CheckCollisionPointRec(
              TileTransformation::positionToScreen(position),
              rectangle);
        }

        void processKeyControl(Transformation& cursorPosition)
        {
            // Store last key
            static int key{};
            int keyPressed{GetKeyPressed()};
            if (keyPressed)
                key = keyPressed;

            Vector2I dir{};

            static Timer timer{CURSOR_MOVE_TICK};
            static Timer delay{CURSOR_MOVE_DELAY};

            // Set direction
            switch (key)
            {
            case KEY_W:
            case KEY_UP:
            {
                if (keyPressed || (delay.hasDelayPassed(IsKeyDown(key)) && timer.hasTimePassed()))
                    dir = V_UP;
            }
            break;

            case KEY_A:
            case KEY_LEFT:
            {
                if (keyPressed || (delay.hasDelayPassed(IsKeyDown(key)) && timer.hasTimePassed()))
                    dir = V_LEFT;
            }
            break;

            case KEY_S:
            case KEY_DOWN:
            {
                if (keyPressed || (delay.hasDelayPassed(IsKeyDown(key)) && timer.hasTimePassed()))
                    dir = V_DOWN;
            }
            break;

            case KEY_D:
            case KEY_RIGHT:
            {
                if (keyPressed || (delay.hasDelayPassed(IsKeyDown(key)) && timer.hasTimePassed()))
                    dir = V_RIGHT;
            }
            break;

            default:
                break;
            }

            // No boost by default
            int factor{1};

            // Apply pan boost factor to cursor movement when shift is pressed
            if (IsKeyDown(KEY_LEFT_SHIFT))
                factor = PAN_BOOST_FACTOR;

            // Check if cursor would go out of screen
            Vector2I newCursorPosition{
              getNewCursorPosition(
                cursorPosition.tilePosition(),
                dir,
                factor)};

            // If new position were out of screen with potential boost applied
            Rectangle renderRectangle{Render::getRenderRectangle().rectangle()};

            if (isOutOfRectangle(
                  newCursorPosition,
                  renderRectangle))
            {
                // Return early if no boost applied
                if (factor == 1)
                    return;

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
                      renderRectangle))
                    return;
            }

            // Set verified position
            cursorPosition.setTilePosition(newCursorPosition);
        }
    }

    void update(Transformation& cursorPosition, bool isMouseControlled)
    {
        // Cursor control
        if (isMouseControlled)
        {
            // Mouse controlled cursor
            cursorPosition.setTilePosition(TileTransformation::getMouseTile());
        }
        else
        {
            processKeyControl(cursorPosition);
        }
    }
}
