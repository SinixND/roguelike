#include "GameScene.h"

#include "Attack.h"
#include "CameraControl.h"
#include "Constants.h"
#include "CursorControl.h"
#include "Entity.h"
#include "Graphic.h"
#include "LayerID.h"
#include "MapOverlay.h"
#include "Movement.h"
#include "Panel.h"
#include "PanelStatus.h"
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

    // Map layer
    for (auto& tile : gameWorld.currentMap().values())
    {
        Render::update(tile.transform.position(), tile.graphic, tile.visibilityID());
    }

    // Map overlay
    for (auto& tile : gameWorld.mapOverlay().values())
    {
        Render::update(tile.transform.position(), tile.graphic, tile.visibilityID());
    }

    // (Single frame) Map overlay
    for (auto& tile : gameWorld.framedMapOverlay().values())
    {
        Render::update(tile.transform.position(), tile.graphic, tile.visibilityID());
    }

    // Object layer
    Render::update(hero.transform.position(), hero.graphic);

    // UI layer
    Render::update(cursor.transform.position(), cursor.graphic);

    EndMode2D();

    // Panels
    //=================================
    PanelStatus::update(gameWorld.currentLevel(), dtb::font(), Panel::panelMap());
    //=================================

    // Draw panel borders
    //=================================
    // Info panel (right)
    DrawRectangleLinesEx(
      Panel::panelTileInfo().rectangle(),
      PANEL_BORDER_WEIGHT,
      DARKGRAY);

    // Info panel (right)
    DrawRectangleLinesEx(
      Panel::panelInfo().rectangle(),
      PANEL_BORDER_WEIGHT,
      DARKGRAY);

    // Status panel (top)
    DrawRectangleLinesEx(
      Panel::panelStatus().rectangle(),
      PANEL_BORDER_WEIGHT,
      DARKGRAY);

    // Log panel (bottom)
    DrawRectangleLinesEx(
      Panel::panelLog().rectangle(),
      PANEL_BORDER_WEIGHT,
      DARKGRAY);

    // Map panel (mid-left)
    DrawRectangleLinesEx(
      Panel::panelMap().rectangle(),
      PANEL_BORDER_WEIGHT,
      DARKGRAY);

    // Under cursor info panel (bottom-right)
    DrawLineEx(
      Panel::panelMap().bottomRight(),
      {static_cast<float>(GetRenderWidth()),
       Panel::panelMap().bottom() + 1},
      PANEL_BORDER_WEIGHT,
      DARKGRAY);
}

void GameScene::postOutput()
{
    hero.transform.resetPositionChanged();

    // Clear path
    gameWorld.framedMapOverlay().clear();
}

void GameScene::deinitialize() {}
