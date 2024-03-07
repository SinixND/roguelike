#include "GameScene.h"

#include "CursorControl.h"
#include "EdgePan.h"
#include "Entity.h"
#include "Graphic.h"
#include "LayerID.h"
#include "MapOverlay.h"
#include "Movement.h"
#include "Pathfinder.h"
#include "Position.h"
#include "Render.h"
#include "RenderID.h"
#include "RuntimeDatabase.h"
#include "Selection.h"
#include "Tile.h"
#include "TileMap.h"
#include "TileTransformation.h"
#include "Unit.h"
#include "UnitMovement.h"
#include "VisibilityID.h"
#include "Vision.h"
#include "World.h"
#include "Zoom.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

namespace
{
    // Variables needed for multiple stepps in main loop
    World gameWorld{};

    Entity cursor{
        Position(),
        Graphic(
            RenderID::cursor,
            LayerID::UI)};

    Unit hero{
        Position(),
        Graphic(
            RenderID::hero,
            LayerID::object),
        Movement(5, 50),
        VisibilityID::visible,
        5};

    bool isInputBlocked{false};
    bool isPathShown{false};
}

void GameScene::initialize()
{
}

void GameScene::processInput()
{
    if (isInputBlocked) return;

    // Toggle debug mode
    if (IsKeyDown(KEY_F1))
        dtb::setDebugMode(!dtb::debugMode());

    // Toggle between mouse or key control for cursor
    static bool mouseActive{true};

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        mouseActive = !mouseActive;

        if (mouseActive)
        {
            ShowCursor();
        }
        else
        {
            HideCursor();
        }
    }

    CursorControl::update(cursor.position, mouseActive);

    EdgePan::update(
        TileTransformation::positionToWorld(cursor.position.tilePosition()),
        mouseActive);

    Zoom::update();

    Selection::update(
        hero,
        cursor.position.tilePosition());

    // Set unit movment target
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        UnitMovement::setTarget(
            gameWorld,
            hero,
            cursor.position);
    }
}

void GameScene::updateState()
{
    static bool moveRangeShown{false};

    static Path path{};

    int condition = // A=isSelected, B=rangeShown
        (hero.isSelected ? (true << 1) : false) +
        (moveRangeShown ? (true << 0) : false);

    switch (condition)
    {
        case 0:
            // 0 0 // not selected, not shown -> Do nothing
            break;

        case 1:
        { // 0 1 // not selected, range shown -> Hide range
            gameWorld.mapOverlay().clear();
            moveRangeShown = false;
            isPathShown = false;
        }
        break;

        case 2:
        { // 1 0 // selected, range not shown -> Show range
            MapOverlay::showUnitRange(
                moveRangeShown,
                hero,
                gameWorld);
        }
        break;

        case 3:
        { // 1 1 // selected, range shown -> Show path
            path = MapOverlay::showPath(
                hero.position.tilePosition(),
                cursor.position.tilePosition(),
                hero.movement.range,
                gameWorld,
                isPathShown);
        }
        break;

        default:
            break;
    }

    UnitMovement::triggerMovement(hero.movement, path, isInputBlocked);

    // Unblock input if target is reached
    UnitMovement::processMovement(hero, isInputBlocked);

    Vision::update(hero, gameWorld.currentMap());
}

void GameScene::renderOutput()
{
    BeginMode2D(dtb::camera());

    // Layer map
    for (auto& tile : gameWorld.currentMap().values())
    {
        Render::update(tile.position.get(), tile.graphic, tile.visibilityID);
    }

    // Layer map overlay
    for (auto& tile : gameWorld.mapOverlay().values())
    {
        Render::update(tile.position.get(), tile.graphic, tile.visibilityID);
    }

    // Layer framed map overlay
    for (auto& tile : gameWorld.framedMapOverlay().values())
    {
        Render::update(tile.position.get(), tile.graphic, tile.visibilityID);
    }

    // Render object layer
    Render::update(hero.position.get(), hero.graphic);

    // Render UI layer
    Render::update(cursor.position.get(), cursor.graphic);

    EndMode2D();

    // Draw text for current level
    //=================================
    const char* currentLevel{TextFormat("Level %i", gameWorld.currentLevel())};

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
}

void GameScene::postOutput()
{
    // Clear path
    gameWorld.framedMapOverlay().clear();
}

void GameScene::deinitialize() {}
