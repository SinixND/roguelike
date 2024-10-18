#include "Panels.h"

#include "Event.h"
#include "GameFont.h"
#include "Logger.h"
#include "Objects.h"
#include "PanelData.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <string>

void Panels::init()
{
    tileInfo
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeWidthLeft(PanelData::INFO_PANEL_WIDTH)
        .resizeHeightTop(PanelData::LOG_PANEL_HEIGHT);

    info_
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(tileInfo.top())
        .resizeWidthLeft(PanelData::INFO_PANEL_WIDTH);

    status
        .setRight(tileInfo.left())
        .setBottom(PanelData::STATUS_PANEL_HEIGHT);

    log
        .setRight(tileInfo.left())
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeHeightTop(PanelData::LOG_PANEL_HEIGHT);

    map
        .setRight(tileInfo.left())
        .setBottom(log.top())
        .setTop(status.bottom());

    snx::PublisherStatic::publish(Event::panelsResized);
}

void Panels::drawStatusPanelContent(int level) const
{
    //* Draw text for current level
    char const* currentLevel{TextFormat("Level %i", level)};

    Font const& font{GuiGetFont()};

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    int fontSpacing{GuiGetStyle(DEFAULT, TEXT_SPACING)};

    Vector2 textDimensions{MeasureTextEx(font, currentLevel, fontSize, fontSpacing)};

    DrawTextEx(
        font,
        currentLevel,
        Vector2{
            ((map.width() / 2.0f) - (textDimensions.x / 2)),
            (fontSize / 2.0f)},
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}

void Panels::drawLogPanelContent() const
{
    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    double lines{(log.height() / (1.5 * fontSize)) - 1};
    for (int i{0}; i < lines; ++i)
    {
        DrawTextEx(
            GameFont::font(),
            snx::Logger::getMessage(i).c_str(),
            Vector2{log.left() + (fontSize / 2.0f), log.bottom() - (fontSize * 1.5f) - (i * 1.5f * fontSize)},
            fontSize,
            0,
            LIGHTGRAY);
    }
}

void Panels::drawTileInfoPanelContent(Objects const& objects, Vector2I const& cursorPosition) const
{
    if (!objects.tags_.contains(cursorPosition))
    {
        return;
    }

    //* Draw tag from tile under cursor
    char const* tag{TextFormat("[%s]", objects.tags_.at(cursorPosition).c_str())};

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};

    DrawTextEx(
        GameFont::font(),
        tag,
        Vector2{
            tileInfo.left() + (fontSize * 0.5f),
            tileInfo.top() + (fontSize * 0.5f)},
        fontSize,
        0,
        LIGHTGRAY);
}

void Panels::drawPanelBorders() const
{
    DrawRectangleLinesEx(
        tileInfo.rectangle(),
        PanelData::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        info_.rectangle(),
        PanelData::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        status.rectangle(),
        PanelData::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        log.rectangle(),
        PanelData::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        map.rectangle(),
        PanelData::PANEL_BORDER_WEIGHT,
        DARKGRAY);
}
