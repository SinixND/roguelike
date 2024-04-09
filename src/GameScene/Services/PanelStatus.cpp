#include "PanelStatus.h"

#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>

void PanelStatus::update(int level, Font const& font, RectangleEx const& mapPanel)
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
            ((mapPanel.width() / 2.0F) - (textDimensions.x / 2)),
            (fontSize / 2.0F)},
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
}