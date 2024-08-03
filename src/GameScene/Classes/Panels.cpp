#include "Panels.h"

#include "GameScene.h"
#include "Event.h"
#include "GameFont.h"
#include "Logger.h"
#include "PanelData.h"
#include "PublisherStatic.h"
#include <raylib.h>
#include <string>

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

    GameScene::eventDispatcher.publish(Event::panelsResized);
}

void Panels::drawLogPanelContent()
{
    auto textSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    auto lines{(log_.height() / (1.5 * textSize)) - 1};
    for (int i{0}; i < lines; ++i)
    {
        std::string message{snx::Logger::getMessage(i)};

        DrawTextEx(
            GameFont::font(),
            message.c_str(),
            {log_.left() + (textSize / 2),
             log_.bottom() - (textSize * 1.5f) - (i * 1.5f * textSize)},
            textSize,
            0,
            LIGHTGRAY);
    }
}

void Panels::drawPanelBorders()
{
    DrawRectangleLinesEx(
        tileInfo_.rectangle(),
        PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        info_.rectangle(),
        PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        status_.rectangle(),
        PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        log_.rectangle(),
        PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        map_.rectangle(),
        PANEL_BORDER_WEIGHT,
        DARKGRAY);
}
