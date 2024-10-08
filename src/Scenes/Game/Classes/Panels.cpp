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

RectangleEx const& Panels::tileInfo() const
{
    return tileInfo_;
}

RectangleEx const& Panels::info() const
{
    return info_;
}

RectangleEx const& Panels::status() const
{
    return status_;
}

RectangleEx const& Panels::log() const
{
    return log_;
}

RectangleEx const& Panels::map() const
{
    return map_;
}

void Panels::init()
{
    tileInfo_
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeWidthLeft(PanelData::INFO_PANEL_WIDTH)
        .resizeHeightTop(PanelData::LOG_PANEL_HEIGHT);

    info_
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(tileInfo_.top())
        .resizeWidthLeft(PanelData::INFO_PANEL_WIDTH);

    status_
        .setRight(tileInfo_.left())
        .setBottom(PanelData::STATUS_PANEL_HEIGHT);

    log_
        .setRight(tileInfo_.left())
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeHeightTop(PanelData::LOG_PANEL_HEIGHT);

    map_
        .setRight(tileInfo_.left())
        .setBottom(log_.top())
        .setTop(status_.bottom());

    snx::PublisherStatic::publish(Event::panelsResized);
}

void Panels::drawStatusPanelContent(int level) const
{
    // Draw text for current level
    char const* currentLevel{TextFormat("Level %i", level)};

    Font const& font{GuiGetFont()};

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    int fontSpacing{GuiGetStyle(DEFAULT, TEXT_SPACING)};

    Vector2 textDimensions{MeasureTextEx(font, currentLevel, fontSize, fontSpacing)};

    DrawTextEx(
        font,
        currentLevel,
        Vector2{
            ((map_.width() / 2.0f) - (textDimensions.x / 2)),
            (fontSize / 2.0f)},
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}

void Panels::drawLogPanelContent() const
{
    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    double lines{(log_.height() / (1.5 * fontSize)) - 1};
    for (int i{0}; i < lines; ++i)
    {
        DrawTextEx(
            GameFont::font(),
            snx::Logger::getMessage(i).c_str(),
            Vector2{log_.left() + (fontSize / 2.0f), log_.bottom() - (fontSize * 1.5f) - (i * 1.5f * fontSize)},
            fontSize,
            0,
            LIGHTGRAY);
    }
}

void Panels::drawTileInfoPanelContent(Objects const& objects, Vector2I const& cursorPosition) const
{
    if (!objects.tags().contains(cursorPosition))
    {
        return;
    }

    // Draw tag from tile under cursor
    char const* tag{TextFormat("[%s]", objects.tag(cursorPosition).c_str())};

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};

    DrawTextEx(
        GameFont::font(),
        tag,
        Vector2{
            tileInfo_.left() + (fontSize * 0.5f),
            tileInfo_.top() + (fontSize * 0.5f)},
        fontSize,
        0,
        LIGHTGRAY);
}

void Panels::drawPanelBorders() const
{
    DrawRectangleLinesEx(
        tileInfo_.rectangle(),
        PanelData::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        info_.rectangle(),
        PanelData::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        status_.rectangle(),
        PanelData::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        log_.rectangle(),
        PanelData::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        map_.rectangle(),
        PanelData::PANEL_BORDER_WEIGHT,
        DARKGRAY);
}
