#include "GameScene.h"

#include "Attack.h"
#include "CameraControl.h"
#include "CursorControl.h"
#include "Entity.h"
#include "Graphic.h"
#include "LayerID.h"
#include "MapOverlay.h"
#include "Movement.h"
#include "Pathfinder.h"
#include "Render.h"
#include "RenderID.h"
#include "RuntimeDatabase.h"
#include "Selection.h"
#include "Tile.h"
#include "TileMap.h"
#include "TileTransformation.h"
#include "Transformation.h"
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
    World gameWorld{};

    Entity cursor{
      Transformation(),
      Graphic(
        RenderID::CURSOR,
        LayerID::UI)};

    Unit hero{
      Transformation(),
      Graphic(
        RenderID::HERO,
        LayerID::OBJECT),
      Movement(5, 50),
      VisibilityID::VISIBLE,
      20,
      Attack(
        1,
        1)};

    bool isInputBlocked{false};
}

void GameScene::initialize()
{
}

void GameScene::processInput()
{
    if (isInputBlocked) return;

    // Toggle debug mode
    if (IsKeyPressed(KEY_F1))
        dtb::toggleDebugMode();

    // Toggle between mouse or key control for cursor
    static bool isMouseControlled{true};

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        isMouseControlled = !isMouseControlled;

        if (isMouseControlled)
        {
            ShowCursor();
        }
        else
        {
            HideCursor();
        }
    }

    // Update cursor
    CursorControl::update(cursor.transform, isMouseControlled);

    // Process edge pan
    CameraControl::edgePan(
      dtb::camera(),
      TileTransformation::positionToWorld(cursor.transform.tilePosition()),
      isMouseControlled);

    // Center on hero
    if (IsKeyPressed(KEY_H))
        CameraControl::centerOnHero(dtb::camera(), hero);

    // Process zoom
    Zoom::update(GetMouseWheelMove(), dtb::camera());

    // Reset Zoom
    if (
      (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
      && (IsKeyPressed(KEY_KP_0) || IsKeyPressed(KEY_ZERO)))
    {
        Zoom::reset(dtb::camera());
    }

    // Select unit
    if (
      IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        Selection::select(
          hero,
          cursor.transform.tilePosition());
    }

    // Deselect unit
    if (
      IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_CAPS_LOCK))
    {
        Selection::deselect(hero);
    }

    // Set unit movment target
    if (
      IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        UnitMovement::setTarget(
          gameWorld,
          hero,
          cursor.transform);
    }
}

void GameScene::updateState()
{
    // Update map overlay
    static bool isRangeShown{false};

    static Path path{};

    if (hero.isSelected())
    {
        if (!isRangeShown)
        {
            MapOverlay::showUnitMoveRange(
              hero,
              gameWorld);

            MapOverlay::showUnitActionRange(
              hero,
              gameWorld);

            isRangeShown = true;
        }
        else // range is shown
        {
            path = MapOverlay::showPath(
              hero.transform.tilePosition(),
              cursor.transform.tilePosition(),
              hero.movement.range(),
              gameWorld);
        }
    }
    else // not selected
    {
        if (isRangeShown)
        {
            gameWorld.mapOverlay().clear();

            isRangeShown = false;
        }
    }

    UnitMovement::triggerMovement(hero.movement, path, isInputBlocked);

    // Unblock input if target is reached
    UnitMovement::processMovement(hero, isInputBlocked);

    if (hero.transform.hasPositionChanged())
        Vision::update(hero, gameWorld.currentMap());
}

void GameScene::renderOutput()
{
    BeginMode2D(dtb::camera());

    // Layer map
    for (auto& tile : gameWorld.currentMap().values())
    {
        Render::update(tile.transform.position(), tile.graphic, tile.visibilityID());
    }

    // Layer map overlay
    for (auto& tile : gameWorld.mapOverlay().values())
    {
        Render::update(tile.transform.position(), tile.graphic, tile.visibilityID());
    }

    // Layer framed map overlay
    for (auto& tile : gameWorld.framedMapOverlay().values())
    {
        Render::update(tile.transform.position(), tile.graphic, tile.visibilityID());
    }

    // Render object layer
    Render::update(hero.transform.position(), hero.graphic);

    // Render UI layer
    Render::update(cursor.transform.position(), cursor.graphic);

    EndMode2D();

    // Draw text for current level
    //=================================
    char const* currentLevel{TextFormat("Level %i", gameWorld.currentLevel())};

    Font const& font{dtb::font()};

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

    // Draw render rectangle
    DrawRectangleLinesEx(Render::getRenderRectangle().rectangle(), 1, DARKGRAY);
}

void GameScene::postOutput()
{
    hero.transform.resetPositionChanged();

    // Clear path
    gameWorld.framedMapOverlay().clear();
}

void GameScene::deinitialize() {}
