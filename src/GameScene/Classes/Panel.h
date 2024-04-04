#ifndef IG20240405000728
#define IG20240405000728

#include "Constants.h"
#include "raylibEx.h"
#include <raylib.h>

namespace Panel
{
    inline RectangleEx panelTileInfo()
    {
        RectangleEx re{};
        re.setRight(static_cast<int>(GetRenderWidth()))
          .setBottom(static_cast<int>(GetRenderHeight()))
          .resizeWidthLeft(INFO_PANEL_WIDTH)
          .resizeHeightTop(LOG_PANEL_HEIGHT);

        return re;
    }
    inline RectangleEx panelInfo()
    {
        RectangleEx re{};
        re.setRight(static_cast<int>(GetRenderWidth()))
          .setBottom(panelTileInfo().top())
          .resizeWidthLeft(INFO_PANEL_WIDTH);

        return re;
    }

    inline RectangleEx panelStatus()
    {
        RectangleEx re{};
        re.setRight(panelTileInfo().left())
          .setBottom(STATUS_PANEL_HEIGHT);

        return re;
    }

    inline RectangleEx panelLog()
    {
        RectangleEx re{};
        re.setRight(panelTileInfo().left())
          .setBottom(static_cast<int>(GetRenderHeight()))
          .resizeHeightTop(LOG_PANEL_HEIGHT);

        return re;
    }

    inline RectangleEx panelMap()
    {
        RectangleEx re{};
        re.setRight(panelInfo().left())
          .setBottom(panelLog().top())
          .setTop(panelStatus().bottom());

        return re;
    }

}

#endif