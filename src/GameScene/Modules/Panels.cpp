#include "Panels.h"

#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

RectangleEx PanelTileInfo::setup()
{
    RectangleEx rect{};
    rect.setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeWidthLeft(Panels::INFO_PANEL_WIDTH)
        .resizeHeightTop(Panels::LOG_PANEL_HEIGHT);

    return rect;
}

void PanelTileInfo::render(TileMap& tileMap, Vector2I cursorPosition, Font const& font)
{
    Tile* tile{tileMap.at(cursorPosition)};

    if (!tile)
    {
        return;
    }

    // Draw tag from tile under cursor
    char const* tag{TextFormat("[%s]", tile->tag().c_str())};

    //* Font const& font{font()};

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    //* int fontSpacing{GuiGetStyle(DEFAULT, TEXT_SPACING)};

    //* Vector2 textDimensions{MeasureTextEx(font, tag, fontSize, fontSpacing)};

    DrawTextEx(
        font,
        tag,
        Vector2AddValue(
            PanelTileInfo::setup().topLeft(),
            fontSize),
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}

RectangleEx PanelInfo::setup()
{
    RectangleEx rect{};
    rect.setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(PanelTileInfo::setup().top())
        .resizeWidthLeft(Panels::INFO_PANEL_WIDTH);

    return rect;
}

RectangleEx PanelStatus::setup()
{
    RectangleEx rect{};
    rect.setRight(PanelTileInfo::setup().left())
        .setBottom(Panels::STATUS_PANEL_HEIGHT);

    return rect;
}

void PanelStatus::render(int level, Font const& font)
{
    // Draw text for current level
    //* char const* currentLevel{TextFormat("Level %i", gameWorld.currentLevel())};
    char const* currentLevel{TextFormat("Level %i", level)};

    //* Font const& font{font()};

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    int fontSpacing{GuiGetStyle(DEFAULT, TEXT_SPACING)};

    Vector2 textDimensions{MeasureTextEx(font, currentLevel, fontSize, fontSpacing)};

    DrawTextEx(
        font,
        currentLevel,
        Vector2{
            ((PanelMap::setup().width() / 2.0F) - (textDimensions.x / 2)),
            (fontSize / 2.0F)},
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}

RectangleEx PanelLog::setup()
{
    RectangleEx rect{};
    rect.setRight(PanelTileInfo::setup().left())
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeHeightTop(Panels::LOG_PANEL_HEIGHT);

    return rect;
}

RectangleEx PanelMap::setup()
{
    RectangleEx rect{};
    rect.setRight(PanelInfo::setup().left())
        .setBottom(PanelLog::setup().top())
        .setTop(PanelStatus::setup().bottom());

    return rect;
}