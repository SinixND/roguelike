#include "UISystems.h"

#include "ColorData.h"
#include "UIData.h"
#include "raylibEx.h"

namespace UISystems
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
