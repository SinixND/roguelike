#include "Panels.h"

#include "Event.h"
#include "PanelData.h"
#include "PublisherStatic.h"
#include <raylib.h>

void Panels::init()
{
    tileInfo_
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeWidthLeft(INFO_PANEL_WIDTH)
        .resizeHeightTop(LOG_PANEL_HEIGHT);

    info_
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(tileInfo_.top())
        .resizeWidthLeft(INFO_PANEL_WIDTH);

    status_
        .setRight(tileInfo_.left())
        .setBottom(STATUS_PANEL_HEIGHT);

    log_
        .setRight(tileInfo_.left())
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeHeightTop(LOG_PANEL_HEIGHT);

    map_
        .setRight(tileInfo_.left())
        .setBottom(log_.top())
        .setTop(status_.bottom());

    snx::Publisher::notify(Event::panelsResized);
}