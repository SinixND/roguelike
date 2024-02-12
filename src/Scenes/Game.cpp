#include "Game.h"

#include "Entity.h"
#include "Invoker.h"
#include "LayerId.h"
#include "Render.h"
#include "RenderId.h"
#include "RuntimeDatabase.h"
#include "Services/Renderer.h"
#include "TileGenerator.h"
#include "TileMap.h"
#include "Unit.h"
#include "Utility.h"
#include "World.h"
#include <iostream>
#include <raygui.h>
#include <raylib.h>

Renderer renderer{};
World world{};
Entity cursor{};
Unit hero{5};

// Command invokers
Invoker renderMapCommands{};
Invoker renderMapOverlayCommands{};

void GameScene::initialize()
{
    cursor.renderData = {
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
    //=============================
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        dtb::Globals::toggleMouseActivated();
    }
    //=============================
}

void GameScene::updateState() {}

void GameScene::renderOutputWorld()
{
    BeginMode2D(dtb::Globals::getCamera());

    // Layer Map
    for (const auto& tile : world.getCurrentMap().getValues())
    {
        Render render{renderer, tile.renderData.renderId_, tile.position_.x, tile.position_.y};

        switch (tile.renderData.layerId_)
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

        renderer.render(tile.renderData.renderId_, tile.position_.x, tile.position_.y);
    }

    // Layer Units
    renderer.render(hero.renderData.renderId_, hero.position_.x, hero.position_.y);

    // Layer UI
    renderer.render(cursor.renderData.renderId_, cursor.position_.x, cursor.position_.y);

    // Call command invokers
    renderMapCommands.executeQueue();
    renderMapOverlayCommands.executeQueue();

    EndMode2D();
}

void GameScene::renderOutput()
{
    renderOutputWorld();

    const char* currentLevel{TextFormat("Level %i", world.getCurrentLevel())};

    Font& font{dtb::Constants::getFont()};
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
}

void GameScene::deinitialize() {}
