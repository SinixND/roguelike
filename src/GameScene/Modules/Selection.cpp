#include "Selection.h"

#include "Unit.h"
#include "raylibEx.h"
#include <raylib.h>

namespace Selection
{
    void select(Unit& unit, Vector2I cursorPosition)
    {
        if (unit.transform.tilePosition() == cursorPosition)
        {
            // Toggle isSelected
            unit.setIsSelected(!unit.isSelected());
        }
    }

    void deselect(Unit& unit)
    {
        unit.setIsSelected(false);
    }
}