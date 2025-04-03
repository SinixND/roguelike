#include "WindowSystem.h"

#include "ColorData.h"
#include "WindowData.h"
#include "raylibEx.h"

namespace WindowSystem
{
    void drawWindowBorder()
    {
        DrawRectangleLinesEx(
            GetWindowRec(),
            WindowData::WINDOW_BORDER_WIDTH,
            ColorData::BORDER
        );
    }
}
