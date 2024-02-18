#include "Game.h"

#include "Constants.h"
#include "EdgePan.h"
#include "GameObject.h"
#include "Pathfinder.h"
#include "Render.h"
#include "RenderId.h"
#include "RuntimeDatabase.h"
#include "TileMap.h"
#include "TilePositionConversion.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"
#include <iostream>
#include <raygui.h>
#include <raylib.h>

World world{};
GameObject cursor{};
Unit hero{};

void GameScene::initialize()
{
    cursor.setRenderId(RENDER_CURSOR);

    hero.setMoveRange(5);
    hero.setRenderId(RENDER_HERO);
}

void GameScene::processInput()
{

    processEdgePan(cursor.position(), hero.position());

    // Toggle between mouse or key control for cursor
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        dtb::Globals::toggleMouseActivated();
    }

    // Cursor control
    if (dtb::Globals::isMouseActivated())
    {
        // Mouse controlled cursor
        setMouseTile(cursor.position());
    }
    else
    {
        // Key controlled cursor
        Vector2Int dir{};

        // Get direction
        switch (GetKeyPressed())
        {
        case KEY_W:
        case KEY_UP:
            if (positionToScreen(cursor.position()).y > 0)
                dir = V_UP;
            break;
        case KEY_A:
        case KEY_LEFT:
            if (positionToScreen(cursor.position()).x > 0)
                dir = V_LEFT;
            break;
        case KEY_S:
        case KEY_DOWN:
            if (positionToScreen(cursor.position()).y < GetRenderHeight())
                dir = V_DOWN;
            break;
        case KEY_D:
        case KEY_RIGHT:
            if (positionToScreen(cursor.position()).x < GetRenderWidth())
                dir = V_RIGHT;
            break;

        default:
            break;
        }

        // Jump when shift is pressed
        int scale{1};
        if (IsKeyDown(KEY_LEFT_SHIFT))
            scale = 4;

        cursor.setPosition(
            {cursor.position().x + (dir.x * scale),
             cursor.position().y + (dir.y * scale)});
    }

    // Select unit
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        if (hero.position() == cursor.position())
        {
            hero.toggleSelected();
            std::cout << "Hero selected: " << hero.selected() << "\n";
        }
    }

    // Deselect unit
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_CAPS_LOCK))
    {
        hero.setSelected(false);
        std::cout << "Hero selected: " << hero.selected() << "\n";
    }
}

void GameScene::updateState()
{
    // Show range of selected unit
    static bool moveRangeShown{false};

    if (hero.selected() && !moveRangeShown)
    {
        moveRangeShown = true;

        // Filter relevant tiles
        for (auto& steppedPositions : filterReachable(world.currentMap(), hero.moveRange(), hero.position()))
        {
            for (auto& steppedPosition : steppedPositions)
            {
                // Create reachable tile
                Tile reachableTile{};
                reachableTile.setPosition(steppedPosition.position);
                reachableTile.setRenderId(RENDER_REACHABLE_TILE);
                reachableTile.setIsSolid(false);

                // Add reachable tile to overlay
                world.mapOverlay().insert(
                    steppedPosition.position,
                    reachableTile);
            }
        }
    }

    // Clear overlay if no unit selected
    if (!hero.selected() && moveRangeShown)
    {
        world.mapOverlay().clear();
        moveRangeShown = false;
    }

    // Show path
    if (moveRangeShown)
    {
        for (auto& steppedPosition : findPath(
                 world.mapOverlay(),
                 hero.position(),
                 cursor.position(),
                 hero.moveRange()))
        {
            Tile pathTile{};
            pathTile.setPosition(steppedPosition.position);
            pathTile.setRenderId(RENDER_PATH_TILE);
            pathTile.setIsSolid(false);

            world.framedMapOverlay().insert(
                steppedPosition.position,
                pathTile);
        }
    }
}

void GameScene::renderOutputWorld()
{
    BeginMode2D(dtb::Globals::camera());

    // Layer map
    for (auto& tile : world.currentMap().values())
    {
        render(
            tile.renderId(),
            tile.position().x,
            tile.position().y);
    }

    // Layer map overlay
    for (auto& tile : world.mapOverlay().values())
    {
        render(
            tile.renderId(),
            tile.position().x,
            tile.position().y);
    }

    // Layer framed map overlay
    for (auto& tile : world.framedMapOverlay().values())
    {
        render(
            tile.renderId(),
            tile.position().x,
            tile.position().y);
    }

    // Render object layer
    render(hero.renderId(), hero.position().x, hero.position().y);

    // Render UI layer
    render(cursor.renderId(), cursor.position().x, cursor.position().y);

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
