#ifndef IG20240308010118
#define IG20240308010118

#include "Unit.h"
#include "raylibEx.h"

namespace Selection
{
    void select(Unit& unit, Vector2I cursorPosition);
    void deselect(Unit& unit);
}

#endif