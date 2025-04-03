#include "UISystem.h"

#include "ColorData.h"
#include "UIData.h"
#include "raylibEx.h"

namespace UISystem
{
    void drawWindowBorder()
    {
        DrawRectangleLinesEx(
            GetWindowRec(),
            UIData::BORDER_WIDTH,
            ColorData::BORDER
        );
    }
}
