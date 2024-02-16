#include "Game.h"

#include "CommandInvoker.h"
#include "GameObject.h"
#include "LayerId.h"
#include "RenderCommand.h"
#include "RenderId.h"
#include "RenderService.h"
#include "RuntimeDatabase.h"
#include "TileMap.h"
#include "TilePositionConversion.h"
#include "Unit.h"
#include "World.h"
#include <iostream>
#include <raygui.h>
#include <raylib.h>

RenderService renderer{};

World world{};
GameObject cursor{};
Unit hero{};

// Command invokers
CommandInvoker renderMapCommands{};
CommandInvoker renderMapOverlayCommands{};

void GameScene::initialize()
{
    cursor.setRenderData({RENDER_CURSOR,
                          LAYER_UI});

    hero.setMoveRange(5);
    hero.setRenderData({RENDER_HERO,
                        LAYER_OBJECT});
}

void GameScene::processInput()
{
    setMouseTile(cursor.position());

    // Toggle between mouse or key control for cursor
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        dtb::Globals::toggleMouseActivated();
    }

    // Select unit
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
    }
}

void GameScene::updateState()
{
    // Show range of selected unit
}

void GameScene::renderOutputWorld()
{
    BeginMode2D(dtb::Globals::camera());

    // Layer Map
    for (auto& tile : world.currentMap().values())
    {
        RenderCommand render{renderer, tile.renderData().renderId_, tile.position().x, tile.position().y};

        switch (tile.renderData().layerId_)
        {
        case LAYER_MAP:
            renderMapCommands.queueCommand(render);
            break;

        case LAYER_MAP_OVERLAY:
            renderMapOverlayCommands.queueCommand(render);
            break;

        default:
            break;
        }

        renderer.render(tile.renderData().renderId_, tile.position().x, tile.position().y);
    }

    // Layer Units
    renderer.render(hero.renderData().renderId_, hero.position().x, hero.position().y);

    // Layer UI
    renderer.render(cursor.renderData().renderId_, cursor.position().x, cursor.position().y);

    // Call command invokers
    renderMapCommands.executeQueue();
    renderMapOverlayCommands.executeQueue();

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

void GameScene::deinitialize() {}
