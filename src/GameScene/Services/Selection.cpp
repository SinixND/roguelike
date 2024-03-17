#include "Selection.h"

#include "Unit.h"
#include "raylibEx.h"
#include <raylib.h>

namespace Selection
{
    void select(Unit& unit, const Vector2i& cursorPosition)
    {
        if (unit.position.tilePosition() == cursorPosition)
        {
            // Toggle isSelected
            unit.isSelected = !unit.isSelected;
        }
    }

    void deselect(Unit& unit)
    {
        unit.isSelected = false;
    }
}