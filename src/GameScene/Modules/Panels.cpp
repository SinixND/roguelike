#include "Panels.h"

#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

auto PanelTileInfo::panel() -> RectangleEx
{
    RectangleEx rect{};
    rect.setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeWidthLeft(Panels::INFO_PANEL_WIDTH)
        .resizeHeightTop(Panels::LOG_PANEL_HEIGHT);

    return rect;
}

void PanelTileInfo::update(TileMap& tileMap, Vector2I cursorPosition, Font const& font)
{
    Tile* tile{tileMap.at(cursorPosition)};

    if (!tile)
    {
        return;
    }

    // Draw tag from tile under cursor
    char const* tag{TextFormat("[%s]", tile->tag().c_str())};

    //* Font const& font{dtb::font()};

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    //* int fontSpacing{GuiGetStyle(DEFAULT, TEXT_SPACING)};

    //* Vector2 textDimensions{MeasureTextEx(font, tag, fontSize, fontSpacing)};

    DrawTextEx(
        font,
        tag,
        Vector2AddValue(
            PanelTileInfo::panel().topLeft(),
            fontSize),
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}

auto PanelInfo::panel() -> RectangleEx
{
    RectangleEx rect{};
    rect.setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(PanelTileInfo::panel().top())
        .resizeWidthLeft(Panels::INFO_PANEL_WIDTH);

    return rect;
}

auto PanelStatus::panel() -> RectangleEx
{
    RectangleEx rect{};
    rect.setRight(PanelTileInfo::panel().left())
        .setBottom(Panels::STATUS_PANEL_HEIGHT);

    return rect;
}

void PanelStatus::update(int level, Font const& font)
{
    // Draw text for current level
    //* char const* currentLevel{TextFormat("Level %i", gameWorld.currentLevel())};
    char const* currentLevel{TextFormat("Level %i", level)};

    //* Font const& font{dtb::font()};

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    int fontSpacing{GuiGetStyle(DEFAULT, TEXT_SPACING)};

    Vector2 textDimensions{MeasureTextEx(font, currentLevel, fontSize, fontSpacing)};

    DrawTextEx(
        font,
        currentLevel,
        Vector2{
            ((PanelMap::panel().width() / 2.0F) - (textDimensions.x / 2)),
            (fontSize / 2.0F)},
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}

auto PanelLog::panel() -> RectangleEx
{
    RectangleEx rect{};
    rect.setRight(PanelTileInfo::panel().left())
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeHeightTop(Panels::LOG_PANEL_HEIGHT);

    return rect;
}

auto PanelMap::panel() -> RectangleEx
{
    RectangleEx rect{};
    rect.setRight(PanelInfo::panel().left())
        .setBottom(PanelLog::panel().top())
        .setTop(PanelStatus::panel().bottom());

    return rect;
}