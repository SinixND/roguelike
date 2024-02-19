#include "CursorMovement.h"
#include "RuntimeDatabase.h"
#include "TilePositionConversion.h"
#include "raylibEx.h"
#include <raylib.h>

void processCursorMovement(Vector2Int& cursorPosition)
{
    // Cursor control
    if (dtb::Configs::isMouseActivated())
    {
        // Mouse controlled cursor
        setMouseTile(cursorPosition);
    }
    else
    {
        // Key controlled cursor
        Vector2Int dir{};

        // Get direction
        switch (GetKeyPressed())
        {
        case KEY_W:
        case KEY_UP:
            if (positionToScreen(cursorPosition).y > 0)
                dir = V_UP;
            break;
        case KEY_A:
        case KEY_LEFT:
            if (positionToScreen(cursorPosition).x > 0)
                dir = V_LEFT;
            break;
        case KEY_S:
        case KEY_DOWN:
            if (positionToScreen(cursorPosition).y < GetRenderHeight())
                dir = V_DOWN;
            break;
        case KEY_D:
        case KEY_RIGHT:
            if (positionToScreen(cursorPosition).x < GetRenderWidth())
                dir = V_RIGHT;
            break;

        default:
            break;
        }

        // Boost when shift is pressed
        int scale{1};
        if (IsKeyDown(KEY_LEFT_SHIFT))
            scale = 4;

        // Check if cursor would go out of screen
        Vector2Int newCursorPosition{cursorPosition.x + (dir.x * scale),
                                     cursorPosition.y + (dir.y * scale)};

        // If out of screen and boost applied
        if (scale > 1 && !CheckCollisionPointRec(positionToScreen(newCursorPosition), GetRenderRec()))
        {
            // Check again without boost
            scale = 1;

            newCursorPosition = {cursorPosition.x + (dir.x * scale),
                                 cursorPosition.y + (dir.y * scale)};

            // If still out of screen
            if (!CheckCollisionPointRec(positionToScreen(newCursorPosition), GetRenderRec()))
                return;
        }

        cursorPosition = newCursorPosition;
    }
};
