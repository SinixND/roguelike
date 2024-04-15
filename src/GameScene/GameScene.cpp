#include "GameScene.h"

#include "CameraControl.h"
#include "CursorControl.h"
#include "GameObject.h"
#include "GamePhase.h"
#include "Graphic.h"
#include "InputMode.h"
#include "MapOverlay.h"
#include "Panels.h"
#include "Render.h"
#include "RuntimeDatabase.h"
#include "Selection.h"
#include "Tile.h"
#include "TileMap.h"
#include "TileTransformation.h"
#include "Transformation.h"
#include "Unit.h"
#include "UnitMovement.h"
#include "Vision.h"
#include "World.h"
#include "Zoom.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

void GameScene::initialize()
{
}

void GameScene::processInput()
{
    if (isInputBlocked_)
    {
        return;
    }

    // Toggle debug mode
    if (IsKeyPressed(KEY_F1))
    {
        dtb::toggleDebugMode();
    }

    // Toggle between mouse or key control for cursor
    InputMode::update();

    // Update cursor
    CursorControl::update(cursor_.transform, InputMode::isMouseControlled());

    // Process edge pan
    CameraControl::edgePan(
        dtb::camera(),
        TileTransformation::positionToWorld(cursor_.transform.tilePosition()),
        InputMode::isMouseControlled());

    // Center on hero
    if (IsKeyPressed(KEY_H))
    {
        CameraControl::centerOnHero(dtb::camera(), hero);
    }

    // Process zoom
    Zoom::update(GetMouseWheelMove(), dtb::camera());

    // Reset Zoom
    if (
        (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
        && (IsKeyPressed(KEY_KP_0) || IsKeyPressed(KEY_ZERO)))
    {
        Zoom::reset(dtb::camera());
    }

    // Branch game phase
    switch (gamePhase_)
    {
    case GamePhase::movementPhase:
        // Select unit
        if (
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
            || IsKeyPressed(KEY_SPACE))
        {
            Selection::select(
                hero,
                cursor_.transform.tilePosition());
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
                gameWorld_,
                hero,
                cursor_.transform);
        }
        break;

    case GamePhase::actionPhase:
        break;

    default:
        break;
    }
}

void GameScene::updateState()
{
    // Update map overlay
    MapOverlay::update(hero, gameWorld_, cursor_);

    UnitMovement::triggerMovement(hero.movement, MapOverlay::path(), isInputBlocked_);

    // Unblock input if target is reached
    UnitMovement::processMovement(hero, isInputBlocked_, gamePhase_);

    if (hero.transform.hasPositionChanged())
    {
        Vision::update(hero, gameWorld_.currentMap());
    }
}

void GameScene::renderOutput()
{
    BeginMode2D(dtb::camera());

    // Map layer
    for (auto& tile : gameWorld_.currentMap().values())
    {
        Render::update(tile.transform.position(), tile.graphic, tile.visibilityID());
    }

    // Map overlay
    for (auto& tile : gameWorld_.mapOverlay().values())
    {
        Render::update(tile.transform.position(), tile.graphic);
    }

    // (Single frame) Map overlay
    for (auto& tile : gameWorld_.framedMapOverlay().values())
    {
        Render::update(tile.transform.position(), tile.graphic);
    }

    // Object layer
    Render::update(hero.transform.position(), hero.graphic);

    // UI layer
    Render::update(cursor_.transform.position(), cursor_.graphic);

    EndMode2D();

    // Panels
    //=================================
    PanelStatus::update(
        gameWorld_.currentLevel(),
        dtb::font());

    PanelTileInfo::update(
        gameWorld_.currentMap(),
        cursor_.transform.tilePosition(),
        dtb::font());
    //=================================

    // Draw panel borders
    //=================================
    // Info panel (right)
    DrawRectangleLinesEx(
        PanelTileInfo::panel().rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Info panel (right)
    DrawRectangleLinesEx(
        PanelInfo::panel().rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Status panel (top)
    DrawRectangleLinesEx(
        PanelStatus::panel().rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Log panel (bottom)
    DrawRectangleLinesEx(
        PanelLog::panel().rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Map panel (mid-left)
    DrawRectangleLinesEx(
        PanelMap::panel().rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Under cursor info panel (bottom-right)
    DrawLineEx(
        PanelMap::panel().bottomRight(),
        {static_cast<float>(GetRenderWidth()),
         PanelMap::panel().bottom() + 1},
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);
}

void GameScene::postOutput()
{
    hero.transform.resetPositionChanged();

    // Clear path
    gameWorld_.framedMapOverlay().clear();
}

void GameScene::deinitialize() {}
