#ifndef IG20240405000728
#define IG20240405000728

#include "Constants.h"
#include "raylibEx.h"
#include <raylib.h>

namespace Panel
{
    inline RectangleEx panelTileInfo()
    {
        RectangleEx rect{};
        rect.setRight(static_cast<int>(GetRenderWidth()))
            .setBottom(static_cast<int>(GetRenderHeight()))
            .resizeWidthLeft(INFO_PANEL_WIDTH)
            .resizeHeightTop(LOG_PANEL_HEIGHT);

        return rect;
    }

    inline RectangleEx panelInfo()
    {
        RectangleEx rect{};
        rect.setRight(static_cast<int>(GetRenderWidth()))
            .setBottom(panelTileInfo().top())
            .resizeWidthLeft(INFO_PANEL_WIDTH);

        return rect;
    }

    inline RectangleEx panelStatus()
    {
        RectangleEx rect{};
        rect.setRight(panelTileInfo().left())
            .setBottom(STATUS_PANEL_HEIGHT);

        return rect;
    }

    inline RectangleEx panelLog()
    {
        RectangleEx rect{};
        rect.setRight(panelTileInfo().left())
            .setBottom(static_cast<int>(GetRenderHeight()))
            .resizeHeightTop(LOG_PANEL_HEIGHT);

        return rect;
    }

    inline RectangleEx panelMap()
    {
        RectangleEx rect{};
        rect.setRight(panelInfo().left())
            .setBottom(panelLog().top())
            .setTop(panelStatus().bottom());

        return rect;
    }
}

#endif