#include "Game.h"

#include "Entity.h"
#include "MapGenerator.h"
#include "RenderId.h"
#include "RuntimeDatabase.h"
#include "Services/Renderer.h"
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

void GameScene::initialize()
{
    cursor.renderId_ = CURSOR;

    hero.renderId_ = HERO;
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
        renderer.render(tile.renderId_, tile.position_.x, tile.position_.y);
    }

    // Layer Units
    renderer.render(hero.renderId_, hero.position_.x, hero.position_.y);

    // Layer UI
    renderer.render(cursor.renderId_, cursor.position_.x, cursor.position_.y);

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
