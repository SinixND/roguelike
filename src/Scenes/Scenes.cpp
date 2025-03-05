#include "Scenes.h"

#include "Colors.h"
#include "raylibEx.h"

namespace ScenesModule
{
    float constexpr BORDER_WIDTH{ 1 };

    void drawWindowBorder()
    {
        DrawRectangleLinesEx(
            GetWindowRec(),
            BORDER_WIDTH,
            Colors::border
        );
    }
}

