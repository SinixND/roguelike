#include "Panels.h"

#include "Event.h"
#include "GameFont.h"
#include "Hero.h"
#include "Logger.h"
#include "Objects.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <string>

void Panels::init()
{
    panelBorderWeight = 1;

    heroInfoPanelWidth =
        // static_cast<float>(
        //     (/*Number of chars per line*/ 15 * GameFont::fontWidth)
        //     + 2 * GameFont::fontWidth)
        MeasureTextEx(
            GameFont::font(),
            "1234567890123456",
            GameFont::fontHeight,
            0)
            .x;

    gameInfoPanelHeight =
        static_cast<float>(
            (/*Number of lines to print*/ 1 * GameFont::fontHeight)
            + GameFont::fontHeight);

    logPanelHeight =
        static_cast<float>(
            (/*Number of lines to print*/ 4 * GameFont::fontHeight)
            + GameFont::fontHeight);

    tileInfo
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeWidthLeft(heroInfoPanelWidth)
        .resizeHeightTop(logPanelHeight);

    heroInfo
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(tileInfo.top())
        .resizeWidthLeft(heroInfoPanelWidth);

    status
        .setRight(tileInfo.left())
        .setBottom(gameInfoPanelHeight);

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

void Panels::drawGameInfoPanelContent(int level) const
{
    //* Draw text for current level
    char const* currentLevel{TextFormat("Level %i", level)};

    Font const& font{GameFont::font()};

    float fontSize{GameFont::fontHeight};

    Vector2 textDimensions{
        MeasureTextEx(
            font,
            currentLevel,
            fontSize,
            0)};

    DrawTextEx(
        font,
        currentLevel,
        Vector2{
            ((0.5f * map.width()) - (0.5f * textDimensions.x)),
            (0.5f * fontSize)},
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}

void Panels::drawHeroInfoPanelContent([[maybe_unused]] Hero const& hero) const
{
    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    /*
    |= HERO ======|
    |HP : 100/100 |
    |ATK: 2       |
    |             |
    |> INVENTORY  |
    |HEA: RUSTY   |
    |_____________|
    */
    DrawTextEx(
        GameFont::font(),
        "|= HERO ======|\n"
        "|             |\n"
        "|_____________|",
        Vector2{
            heroInfo.left() + (0.5f * GameFont::fontWidth),
            heroInfo.top() + (0.5f * GameFont::fontHeight)},
        fontSize,
        0,
        LIGHTGRAY);
}

void Panels::drawTileInfoPanelContent(
    Objects const& objects,
    Vector2I const& cursorPosition) const
{
    if (!objects.names.contains(cursorPosition))
    {
        return;
    }

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};

    //* Draw tag and action from tile under cursor
    DrawTextEx(
        GameFont::font(),
        TextFormat("Name: %s\nAction: %s", objects.names.at(cursorPosition).c_str(), objects.actions.at(cursorPosition).c_str()),
        Vector2{
            tileInfo.left() + (0.5f * GameFont::fontWidth),
            tileInfo.top() + (0.5f * fontSize)},
        fontSize,
        0,
        LIGHTGRAY);
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
            Vector2{
                log.left() + (0.5f * fontSize),
                log.bottom() - (fontSize * 1.5f) - (i * 1.5f * fontSize)},
            fontSize,
            0,
            LIGHTGRAY);
    }
}

void Panels::drawPanelBorders() const
{
    DrawRectangleLinesEx(
        tileInfo.rectangle(),
        panelBorderWeight,
        Color{25, 25, 25, 255});

    DrawRectangleLinesEx(
        heroInfo.rectangle(),
        panelBorderWeight,
        Color{25, 25, 25, 255});

    DrawRectangleLinesEx(
        status.rectangle(),
        panelBorderWeight,
        Color{25, 25, 25, 255});

    DrawRectangleLinesEx(
        log.rectangle(),
        panelBorderWeight,
        Color{25, 25, 25, 255});

    DrawRectangleLinesEx(
        map.rectangle(),
        panelBorderWeight,
        Color{25, 25, 25, 255});
}
