#include "Game.h"

#include "DirectionVector.h"
#include "GameObject.h"
#include "Pathfinder.h"
#include "RenderId.h"
#include "Renderer.h"
#include "RuntimeDatabase.h"
#include "TileMap.h"
#include "TilePositionConversion.h"
#include "Unit.h"
#include "World.h"
#include "raylibEx.h"
#include <iostream>
#include <raygui.h>
#include <raylib.h>

Renderer renderer{};

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
        switch (GetKeyPressed())
        {
        case KEY_W:
            cursor.setPosition(Vector2Add(cursor.position(), V_UP));
            break;
        case KEY_A:
            cursor.setPosition(Vector2Add(cursor.position(), V_LEFT));
            break;
        case KEY_S:
            cursor.setPosition(Vector2Add(cursor.position(), V_DOWN));
            break;
        case KEY_D:
            cursor.setPosition(Vector2Add(cursor.position(), V_RIGHT));
            break;

        default:
            break;
        }
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
        for (auto& steppedPosition : findPath(world.mapOverlay(), hero.position(), cursor.position(), hero.moveRange()))
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
        renderer.render(
            tile.renderId(),
            tile.position().x,
            tile.position().y);
    }

    // Layer map overlay
    for (auto& tile : world.mapOverlay().values())
    {
        renderer.render(
            tile.renderId(),
            tile.position().x,
            tile.position().y);
    }

    // Layer framed map overlay
    for (auto& tile : world.framedMapOverlay().values())
    {
        renderer.render(
            tile.renderId(),
            tile.position().x,
            tile.position().y);
    }

    // Render object layer
    renderer.render(hero.renderId(), hero.position().x, hero.position().y);

    // Render UI layer
    renderer.render(cursor.renderId(), cursor.position().x, cursor.position().y);

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
}

void GameScene::postOutput()
{
    // Clear path
    world.framedMapOverlay().clear();
}

void GameScene::deinitialize() {}
