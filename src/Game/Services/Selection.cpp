#include "Selection.h"
#include <raylib.h>

void processSelection(Unit& unit, const Vector2Int& cursorPosition)
{
    // Select unit
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        if (unit.position() == cursorPosition)
        {
            unit.toggleSelected();
        }
    }

    // Deselect unit
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_CAPS_LOCK))
    {
        unit.setSelected(false);
    }
};