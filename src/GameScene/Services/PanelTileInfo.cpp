#include "PanelTileInfo.h"

#include "Tile.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

void PanelTileInfo::update(TileMap& tileMap, Vector2I cursorPosition, Font const& font, RectangleEx const& panel)
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
            panel.topLeft(),
            fontSize),
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}
