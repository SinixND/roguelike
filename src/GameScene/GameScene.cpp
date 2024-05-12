#include "GameScene.h"

#include "CameraControl.h"
#include "CursorControl.h"
#include "DebugMode.h"
#include "Enums/RenderID.h"
#include "GameObject.h"
#include "GamePhase.h"
#include "InputMode.h"
#include "MapOverlay.h"
#include "Panels.h"
#include "RNG.h"
#include "Render.h"
#include "Selection.h"
#include "TileTransformation.h"
#include "UnitMovement.h"
#include "Vision.h"
#include "Zoom.h"
#include <iostream>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

void GameScene::initialize()
{
    // Seed Random number generator
    if (DebugMode::debugMode())
    {
        snx::RNG::seed(1);
    }

    // Textures
    // Load texture atlas
    textures_.loadAtlas("TextureAtlas.png");

    // Register textures
    textures_.registerTexture(RenderID::INVISIBLE, {0, 0});
    textures_.registerTexture(RenderID::CURSOR, {35, 0});
    textures_.registerTexture(RenderID::HERO, {70, 0});
    textures_.registerTexture(RenderID::WALL, {105, 0});
    textures_.registerTexture(RenderID::FLOOR, {0, 35});
    textures_.registerTexture(RenderID::REACHABLE, {35, 35});
    textures_.registerTexture(RenderID::PATH, {70, 35});
    textures_.registerTexture(RenderID::ATTACKABLE, {105, 35});
    textures_.registerTexture(RenderID::SUPPORTABLE, {0, 70});
    textures_.registerTexture(RenderID::NEXT_LEVEL, {35, 70});
}

void GameScene::update()
{

    processInput();
    updateState();

    BeginDrawing();

    ClearBackground(BACKGROUND_COLOR);

    renderOutput();

    // Draw simple frame
    DrawRectangleLinesEx(
        Rectangle{
            0,
            0,
            static_cast<float>(GetRenderWidth()),
            static_cast<float>(GetRenderHeight())},
        BORDER_WIDTH,
        BORDER_COLOR);

    if (DebugMode::debugMode())
    {
        DrawFPS(0, 0);
    }

    EndDrawing();

    postOutput();
}

void GameScene::deinitialize()
{
    // Unload fonts
    gameFont_.unloadFont();

    // Unload texture atlas
    textures_.unloadAtlas();
}

void GameScene::processInput()
{
    if (isInputBlocked_)
    {
        return;
    }

    // Toggle cheatMode
    if (IsKeyPressed(KEY_F1))
    {
        cheatMode_.toggle();
    }

    // Toggle debugMode
    if (IsKeyPressed(KEY_F10))
    {
        DebugMode::toggleDebugMode();
    }

    // Toggle between mouse or key control for cursor
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        InputMode::update();
    }

    // Update cursor
    CursorControl::update(cursor_.positionComponent, camera_, InputMode::isMouseControlled());

    // Process edge pan
    CameraControl::edgePan(
        camera_,
        TileTransformation::positionToWorld(cursor_.positionComponent.tilePosition()),
        InputMode::isMouseControlled(),
        gameWorld_.mapSize_());

    // Center on hero
    if (IsKeyPressed(KEY_H))
    {
        CameraControl::centerOnHero(camera_, hero);
    }

    // Process zoom
    Zoom::update(GetMouseWheelMove(), camera_);

    // Reset Zoom
    if (
        (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
        && (IsKeyPressed(KEY_KP_0) || IsKeyPressed(KEY_ZERO)))
    {
        Zoom::reset(camera_);
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
                cursor_.positionComponent.tilePosition());
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
                cursor_.positionComponent);
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

    UnitMovement::triggerMovement(hero.movementComponent, MapOverlay::path(), isInputBlocked_);

    // Unblock input if target is reached
    UnitMovement::processMovement(hero, isInputBlocked_, gamePhase_);

    if (hero.positionComponent.hasPositionChanged())
    {
        Vision::update(hero, gameWorld_.currentMap());
    }
}

void GameScene::renderOutput()
{
    BeginMode2D(camera_);

    // Map layer
    for (auto& tile : gameWorld_.currentMap().values())
    {
        Render::update(
            tile.positionComponent.renderPosition(),
            tile.graphicComponent,
            camera_,
            textures_,
            cheatMode_(),
            tile.visibilityID());
    }

    // Map overlay
    for (auto& tile : gameWorld_.mapOverlay().values())
    {
        Render::update(
            tile.positionComponent.renderPosition(),
            tile.graphicComponent,
            camera_,
            textures_,
            cheatMode_());
    }

    // (Single frame) Map overlay
    for (auto& tile : gameWorld_.framedMapOverlay().values())
    {
        Render::update(
            tile.positionComponent.renderPosition(),
            tile.graphicComponent,
            camera_,
            textures_,
            cheatMode_());
    }

    // Object layer
    Render::update(
        hero.positionComponent.renderPosition(),
        hero.graphicComponent,
        camera_,
        textures_,
        cheatMode_());

    // UI layer
    Render::update(
        cursor_.positionComponent.renderPosition(),
        cursor_.graphicComponent,
        camera_,
        textures_,
        cheatMode_());

    EndMode2D();

    // Panels
    //=================================
    PanelStatus::render(
        gameWorld_.currentMapLevel(),
        gameFont_());

    PanelTileInfo::render(
        gameWorld_.currentMap(),
        cursor_.positionComponent.tilePosition(),
        gameFont_());
    //=================================

    // Draw panel borders
    //=================================
    // Info panel (right)
    DrawRectangleLinesEx(
        PanelTileInfo::setup().rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Info panel (right)
    DrawRectangleLinesEx(
        PanelInfo::setup().rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Status panel (top)
    DrawRectangleLinesEx(
        PanelStatus::setup().rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Log panel (bottom)
    DrawRectangleLinesEx(
        PanelLog::setup().rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Map panel (mid-left)
    DrawRectangleLinesEx(
        PanelMap::setup().rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Under cursor info panel (bottom-right)
    DrawLineEx(
        PanelMap::setup().bottomRight(),
        {static_cast<float>(GetRenderWidth()),
         PanelMap::setup().bottom() + 1},
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    std::cout
        << "c offset: " << camera_.offset.x << ", " << camera_.offset.y << "\n"
        << "c target: " << camera_.target.x << ", " << camera_.target.y << "\n";
}

void GameScene::postOutput()
{
    hero.positionComponent.resetPositionChanged();

    // Clear path
    gameWorld_.framedMapOverlay().clear();
}
