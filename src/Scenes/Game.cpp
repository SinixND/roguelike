#include "Game.h"

#include "Constants.h"
#include "CursorMovement.h"
#include "EdgePan.h"
#include "GameObject.h"
#include "Graphic.h"
#include "Pathfinder.h"
#include "RuntimeDatabase.h"
#include "Selection.h"
#include "ShowPath.h"
#include "ShowUnitRange.h"
#include "TileMap.h"
#include "TilePositionConversion.h"
#include "Unit.h"
#include "World.h"
#include "Zoom.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <vector>

World world{};
GameObject cursor{};
Unit hero{};

void GameScene::initialize()
{
    cursor.graphic().setRenderID(RenderID::cursor);

    hero.move().setRange(5);
    hero.graphic().setRenderID(RenderID::hero);
}

void GameScene::processInput()
{
    // Toggle between mouse or key control for cursor
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        dtb::Configs::toggleMouseActivated();
    }

    processCursorMovement(cursor.position());

    processEdgePan(
        positionToWorld(cursor.position()),
        positionToWorld(hero.position()));

    processZoom();

    processSelection(
        hero,
        cursor.position());

    // Set unit movment target
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        for (auto& tile : world.framedMapOverlay().values())
        {
            if (!Vector2IntEquals(tile.position(), cursor.position()))
                continue;

            // Set movement target
        }
    }
}

void GameScene::updateState()
{
    static bool moveRangeShown{false};

    showUnitRange(
        moveRangeShown,
        hero,
        world);

    // Clear overlay if no unit selected
    if (!hero.selected() && moveRangeShown)
    {
        world.mapOverlay().clear();
        moveRangeShown = false;
        dtb::Globals::setPathShown(false);
    }

    showPath(
        moveRangeShown,
        hero.position(),
        cursor.position(),
        hero.move().range(),
        world);
}

void GameScene::renderOutputWorld()
{
    BeginMode2D(dtb::Globals::camera());

    // Layer map
    for (auto& tile : world.currentMap().values())
    {
        tile.graphic().render(tile.position());
    }

    // Layer map overlay
    for (auto& tile : world.mapOverlay().values())
    {
        tile.graphic().render(tile.position());
    }

    // Layer framed map overlay
    for (auto& tile : world.framedMapOverlay().values())
    {
        tile.graphic().render(tile.position());
    }

    // Render object layer
    hero.graphic().render(hero.position());

    // Render UI layer
    cursor.graphic().render(cursor.position());

    EndMode2D();
}

void GameScene::renderOutput()
{
    renderOutputWorld();

    // Draw text for current level
    //=================================
    const char* currentLevel{TextFormat("Level %i", world.currentLevel())};

    Font& font{dtb::Constants::font()};
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

void GameScene::postOutput()
{
    // Clear path
    world.framedMapOverlay().clear();
}

void GameScene::deinitialize() {}
