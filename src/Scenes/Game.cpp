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
Unit hero{5};

// Command invokers
CommandInvoker renderMapCommands{};
CommandInvoker renderMapOverlayCommands{};

void GameScene::initialize()
{
    cursor.renderData_ = {
        RENDER_CURSOR,
        LAYER_UI};

    hero.renderData = {
        RENDER_HERO,
        LAYER_OBJECT};
}

void GameScene::processInput()
{
    setMouseTile(cursor.position_);

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
    for (const auto& tile : world.currentMap().values())
    {
        RenderCommand render{renderer, tile.renderData_.renderId_, tile.position_.x, tile.position_.y};

        switch (tile.renderData_.layerId_)
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

        renderer.render(tile.renderData_.renderId_, tile.position_.x, tile.position_.y);
    }

    // Layer Units
    renderer.render(hero.renderData.renderId_, hero.position_.x, hero.position_.y);

    // Layer UI
    renderer.render(cursor.renderData_.renderId_, cursor.position_.x, cursor.position_.y);

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
