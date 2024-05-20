#include "Panels.h"

#include "TextureData.h"
#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

void PanelTileInfo::udpate(RectangleEx* panelTileInfo)
{
    (*panelTileInfo)
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeWidthLeft(Panels::INFO_PANEL_WIDTH)
        .resizeHeightTop(Panels::LOG_PANEL_HEIGHT);
}

void PanelTileInfo::render(RectangleEx const& panelTileInfo, TileMap& tileMap, Vector2I cursorPosition, Font const& font)
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
            panelTileInfo.topLeft(),
            fontSize),
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}

void PanelInfo::update(RectangleEx* panelInfo, RectangleEx const& panelTileInfo)
{
    (*panelInfo)
        .setRight(static_cast<int>(GetRenderWidth()))
        .setBottom(panelTileInfo.top())
        .resizeWidthLeft(Panels::INFO_PANEL_WIDTH);
}

void PanelStatus::update(RectangleEx* panelStatus, RectangleEx const& panelTileInfo)
{
    (*panelStatus)
        .setRight(panelTileInfo.left())
        .setBottom(Panels::STATUS_PANEL_HEIGHT);
}

void PanelStatus::render(RectangleEx const& panelMap, int level, Font const& font)
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
            ((panelMap.width() / 2.0F) - (textDimensions.x / 2)),
            (fontSize / 2.0F)},
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}

void PanelLog::update(RectangleEx* panelLog, RectangleEx const& panelTileInfo)
{
    (*panelLog)
        .setRight(panelTileInfo.left())
        .setBottom(static_cast<int>(GetRenderHeight()))
        .resizeHeightTop(Panels::LOG_PANEL_HEIGHT);
}

void PanelMap::update(
    RectangleEx* panelMap,
    RectangleEx const& panelTileInfo,
    RectangleEx const& panelLog,
    RectangleEx const& panelStatus)
{
    (*panelMap)
        .setRight(panelTileInfo.left())
        .setBottom(panelLog.top())
        .setTop(panelStatus.bottom());
}

void PanelMapExtended::update(RectangleEx* panelMapExtended, RectangleEx const& panelMap)
{
    *panelMapExtended = panelMap;
    (*panelMapExtended)
        .offsetLeft(-TextureData::TILE_SIZE)
        .offsetTop(-TextureData::TILE_SIZE)
        .offsetRight(TextureData::TILE_SIZE)
        .offsetBottom(TextureData::TILE_SIZE);
}

void Panels::SubUpdatePanels::onNotify()
{
    PanelTileInfo::udpate(&panelTileInfo_);

    PanelInfo::update(
        &panelInfo_,
        panelTileInfo_);

    PanelStatus::update(
        &panelStatus_,
        panelTileInfo_);

    PanelLog::update(
        &panelLog_,
        panelTileInfo_);

    PanelMap::update(
        &panelMap_,
        panelTileInfo_,
        panelLog_,
        panelStatus_);

    PanelMapExtended::update(
        &panelMapExtended_,
        panelMap_);
}