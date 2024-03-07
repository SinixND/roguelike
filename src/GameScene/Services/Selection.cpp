#include "Selection.h"

#include "Unit.h"
#include "raylibEx.h"
#include <raylib.h>

namespace Selection
{
    void update(Unit& unit, const Vector2i& cursorPosition)
    {
        // Select unit
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
        {
            if (unit.position.tilePosition() == cursorPosition)
            {
                // Toggle isSelected
                unit.isSelected = !unit.isSelected;
            }
        }

        // Deselect unit
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_CAPS_LOCK))
        {
            unit.isSelected = false;
        }
    }
}