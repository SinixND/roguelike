#include "CursorControl.h"

#include "Constants.h"
#include "Position.h"
#include "TileTransformation.h"
#include "raylibEx.h"
#include <raylib.h>

namespace CursorControl
{
    namespace
    {
        void processKeyControl(Position& cursorPosition);
    }

    void update(Position& cursorPosition, bool mouseActive)
    {
        // Cursor control
        if (mouseActive)
        {
            // Mouse controlled cursor
            cursorPosition.setTilePosition(TileTransformation::getMouseTile());
        }
        else
        {
            processKeyControl(cursorPosition);
        }
    }

    namespace
    {
        void processKeyControl(Position& cursorPosition)
        {
            Vector2i dir{};

            // Get direction
            switch (GetKeyPressed())
            {
                case KEY_W:
                case KEY_UP:
                {
                    if (TileTransformation::positionToScreen(cursorPosition.tilePosition()).y > 0)
                        dir = V_UP;
                }
                break;

                case KEY_A:
                case KEY_LEFT:
                {
                    if (TileTransformation::positionToScreen(cursorPosition.tilePosition()).x > 0)
                        dir = V_LEFT;
                }
                break;

                case KEY_S:
                case KEY_DOWN:
                {
                    if (TileTransformation::positionToScreen(cursorPosition.tilePosition()).y < GetRenderHeight())
                        dir = V_DOWN;
                }
                break;

                case KEY_D:
                case KEY_RIGHT:
                {
                    if (TileTransformation::positionToScreen(cursorPosition.tilePosition()).x < GetRenderWidth())
                        dir = V_RIGHT;
                }
                break;

                default:
                    break;
            }

            // Boost when shift is pressed
            int scale{1};
            if (IsKeyDown(KEY_LEFT_SHIFT))
                scale = 4;

            // Check if cursor would go out of screen
            Vector2i newCursorPosition{cursorPosition.tilePosition().x + (dir.x * scale),
                                       cursorPosition.tilePosition().y + (dir.y * scale)};

            // If out of screen and boost applied
            if (scale > 1 && !CheckCollisionPointRec(TileTransformation::positionToScreen(newCursorPosition), GetRenderRec()))
            {
                // Check again without boost
                scale = 1;

                newCursorPosition = {cursorPosition.tilePosition().x + (dir.x * scale),
                                     cursorPosition.tilePosition().y + (dir.y * scale)};

                // If still out of screen
                if (!CheckCollisionPointRec(TileTransformation::positionToScreen(newCursorPosition), GetRenderRec()))
                    return;
            }

            cursorPosition.setTilePosition(newCursorPosition);
        }
    }
}
