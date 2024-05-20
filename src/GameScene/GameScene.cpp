#include "GameScene.h"

#include "CameraControl.h"
#include "CursorControl.h"
#include "DebugMode.h"
#include "Directions.h"
#include "Enums/RenderID.h"
#include "Event.h"
#include "GameObject.h"
#include "InputMode.h"
#include "MapOverlay.h"
#include "Panels.h"
#include "RNG.h"
#include "Render.h"
#include "Selection.h"
#include "TileTransformation.h"
#include "UnitMovement.h"
#include "Vision.h"
#include "World.h"
#include "Zoom.h"
#include "raylibEx.h"
#include <memory>
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

    // Attach subscribers
    publisher_.addSubscriber(
        std::make_shared<Panels::SubUpdatePanels>(
            Event::windowResized,
            panelTileInfo_,
            panelInfo_,
            panelStatus_,
            panelLog_,
            panelMap_,
            panelMapExtended_));

    publisher_.addSubscriber(
        std::make_shared<CameraControl::SubUpdateCameraOffset>(
            Event::windowResized,
            camera_,
            panelMap_));

    publisher_.addSubscriber(
        std::make_shared<World::SubUpdateRenderTiles>(
            Event::cameraChanged,
            gameWorld_,
            camera_,
            panelMap_));

    // Trigger events to initialize
    publisher_.notify(Event::windowResized);

    // Camera
    camera_ = {
        Vector2Scale(GetDisplaySize(), 0.5f),
        Directions::V_NULL,
        0,
        1};

    gameWorld_.initTilesToRender(
        camera_,
        panelMapExtended_);
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
    CursorControl::update(&cursor_.positionComponent, camera_, InputMode::isMouseControlled(), panelMap_);

    // Process edge pan
    CameraControl::edgePan(
        &camera_,
        TileTransformation::positionToWorld(cursor_.positionComponent.tilePosition()),
        InputMode::isMouseControlled(),
        gameWorld_.mapSize_(),
        panelMap_,
        publisher_);

    // Center on hero
    if (IsKeyPressed(KEY_H))
    {
        CameraControl::centerOnHero(&camera_, gameWorld_.hero);
    }

    // Process zoom
    Zoom::update(GetMouseWheelMove(), &camera_);

    // Reset Zoom
    if (
        (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL))
        && (IsKeyPressed(KEY_KP_0) || IsKeyPressed(KEY_ZERO)))
    {
        Zoom::reset(&camera_);
    }

    // Branch game phase
    // Select unit
    if (
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)
        || IsKeyPressed(KEY_SPACE))
    {
        Selection::select(
            &gameWorld_.hero,
            cursor_.positionComponent.tilePosition());
    }

    // Deselect unit
    if (
        IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_CAPS_LOCK))
    {
        Selection::deselect(&gameWorld_.hero);
    }

    // Set unit movment target
    if (
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_SPACE))
    {
        UnitMovement::setTarget(
            gameWorld_,
            &gameWorld_.hero,
            cursor_.positionComponent);
    }
}

void GameScene::updateState()
{
    // Publish window resize event
    if (IsWindowResized())
    {
        publisher_.notify(Event::windowResized);
    }

    // Update map overlay
    MapOverlay::update(gameWorld_.hero, &gameWorld_, cursor_);

    UnitMovement::triggerMovement(&gameWorld_.hero.movementComponent, MapOverlay::path(), isInputBlocked_);

    // Unblock input if target is reached
    UnitMovement::processMovement(&gameWorld_.hero, isInputBlocked_);

    if (gameWorld_.hero.positionComponent.hasPositionChanged())
    {
        Vision::update(gameWorld_.hero, gameWorld_.currentMap());
    }
}

void GameScene::renderOutput()
{
    BeginMode2D(camera_);
    BeginScissorMode(
        panelMap_.left(),
        panelMap_.top(),
        panelMap_.width(),
        panelMap_.height());

    // Map layer
    for (auto& tile : gameWorld_.tilesToRender().values())
    {
        Render::update(
            tile.positionComponent.renderPosition(),
            tile.graphicComponent,
            //* camera_,
            textures_,
            cheatMode_(),
            //* panelMapExtended_,
            tile.visibilityID());
    }

    // Map overlay
    for (auto& tile : gameWorld_.mapOverlay().values())
    {
        Render::update(
            tile.positionComponent.renderPosition(),
            tile.graphicComponent,
            //* camera_,
            textures_,
            cheatMode_() //* ,
            //* panelMapExtended_
        );
    }

    // (Single frame) Map overlay
    for (auto& tile : gameWorld_.framedMapOverlay().values())
    {
        Render::update(
            tile.positionComponent.renderPosition(),
            tile.graphicComponent,
            //* camera_,
            textures_,
            cheatMode_() //* ,
            //* panelMapExtended_
        );
    }

    // Object layer
    Render::update(
        gameWorld_.hero.positionComponent.renderPosition(),
        gameWorld_.hero.graphicComponent,
        //* camera_,
        textures_,
        cheatMode_() //* ,
        //* panelMapExtended_
    );

    // UI layer
    Render::update(
        cursor_.positionComponent.renderPosition(),
        cursor_.graphicComponent,
        //* camera_,
        textures_,
        cheatMode_() //* ,
        //* panelMapExtended_
    );

    EndScissorMode();
    EndMode2D();

    // Panels
    //=================================
    PanelStatus::render(
        panelTileInfo_,
        gameWorld_.currentMapLevel(),
        gameFont_());

    PanelTileInfo::render(
        panelTileInfo_,
        gameWorld_.currentMap(),
        cursor_.positionComponent.tilePosition(),
        gameFont_());
    //=================================

    // Draw panel borders
    //=================================
    // Tile info panel (right)
    DrawRectangleLinesEx(
        panelTileInfo_.rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Info panel (right)
    DrawRectangleLinesEx(
        panelInfo_.rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Status panel (top)
    DrawRectangleLinesEx(
        panelStatus_.rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Log panel (bottom)
    DrawRectangleLinesEx(
        panelLog_.rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Map panel (mid-left)
    DrawRectangleLinesEx(
        panelMap_.rectangle(),
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);

    // Under cursor info panel (bottom-right)
    DrawLineEx(
        panelMap_.bottomRight(),
        {static_cast<float>(GetRenderWidth()),
         panelMap_.bottom() + 1},
        Panels::PANEL_BORDER_WEIGHT,
        DARKGRAY);
}

void GameScene::postOutput()
{
    gameWorld_.hero.positionComponent.resetPositionChanged();

    // Clear path
    gameWorld_.framedMapOverlay().clear();
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
