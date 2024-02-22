#include "Game.h"

#include "Render.h"
#include "RuntimeDatabase.h"
#include <raygui.h>

void GameScene::renderOutput()
{
    BeginMode2D(dtb::Globals::camera());

    // Layer map
    for (auto& tile : world.currentMap().values())
    {
        render(tile.renderID, tile.position);
    }

    // Layer map overlay
    for (auto& tile : world.mapOverlay().values())
    {
        render(tile.renderID, tile.position);
    }

    // Layer framed map overlay
    for (auto& tile : world.framedMapOverlay().values())
    {
        render(tile.renderID, tile.position);
    }

    // Render object layer
    render(hero.renderID, hero.position);

    // Render UI layer
    render(cursor.renderID, cursor.position);

    EndMode2D();

    // Draw text for current level
    //=================================
    const char* currentLevel{TextFormat("Level %i", world.currentLevel())};

    Font& font{dtb::font()};

    int fontSize{GuiGetStyle(DEFAULT, TEXT_SIZE)};
    int fontSpacing{GuiGetStyle(DEFAULT, TEXT_SPACING)};

    Vector2 textDimensions{MeasureTextEx(font, currentLevel, fontSize, fontSpacing)};

    DrawTextEx(
        font,
        currentLevel,
        Vector2{
            ((GetRenderWidth() / 2.0f) - (textDimensions.x / 2)),
            10},
        fontSize,
        GuiGetStyle(DEFAULT, TEXT_SPACING),
        RAYWHITE);
    //=================================

    if (dtb::Configs::debugMode())
    {
        DrawRectangleLinesEx(
            dtb::Constants::cursorDeadzone(),
            1,
            RED);

        DrawLineV(
            {GetRenderWidth() / 2.0f, 0},
            {GetRenderWidth() / 2.0f, static_cast<float>(GetRenderHeight())},
            RED);

        DrawLineV(
            {0, GetRenderHeight() / 2.0f},
            {static_cast<float>(GetRenderWidth()), GetRenderHeight() / 2.0f},
            RED);
    }
}
