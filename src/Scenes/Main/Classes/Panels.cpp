#include "Panels.h"

#include "Event.h"
#include "GameFont.h"
#include "Logger.h"
#include "Objects.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raygui.h>
#include <raymath.h>
#include <string>

void Panels::init()
{
    panelBorderWeight = 1;

    infoPanelWidth =
        // static_cast<float>(
        //     (/*Number of chars per line*/ 15 * GameFont::fontWidth)
        //     + 2 * GameFont::fontWidth)
        MeasureTextEx(
            GameFont::font(),
            "123456789012345", 
            GameFont::fontHeight,
            GameFont::fontWidth).x ;

    statusPanelHeight =
        static_cast<float>(
            (/*Number of lines to print*/ 1 * (1.5f * GameFont::fontHeight))
            + (0.5f * GameFont::fontHeight)) ;

    logPanelHeight =
        static_cast<float>(
            (/*Number of lines to print*/ 4 * (1.5f * GameFont::fontHeight))
            + (0.5f * GameFont::fontHeight)) ;


    tileInfo
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeWidthLeft(infoPanelWidth)
        .resizeHeightTop(logPanelHeight);

    info
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(tileInfo.top())
        .resizeWidthLeft(infoPanelWidth);

    status
        .setRight(tileInfo.left())
        .setBottom(statusPanelHeight);

    log
        .setRight(tileInfo.left())
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeHeightTop(logPanelHeight);

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

    Font const& font{GameFont::font()};

    int fontSize{static_cast<int>(GameFont::fontHeight)};
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
    if (!objects.tags.contains(cursorPosition))
    {
        return;
    }

    //* Draw tag from tile under cursor
    char const* tag{TextFormat("[%s]", objects.tags.at(cursorPosition).c_str())};

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
        panelBorderWeight,
        DARKGRAY);

    DrawRectangleLinesEx(
        info.rectangle(),
        panelBorderWeight,
        DARKGRAY);

    DrawRectangleLinesEx(
        status.rectangle(),
        panelBorderWeight,
        DARKGRAY);

    DrawRectangleLinesEx(
        log.rectangle(),
        panelBorderWeight,
        DARKGRAY);

    DrawRectangleLinesEx(
        map.rectangle(),
        panelBorderWeight,
        DARKGRAY);
}
