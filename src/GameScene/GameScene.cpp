#include "GameScene.h"

#include "Cursor.h"
#include "DeveloperMode.h"
#include "Observer/Event.h"
#include "PanelData.h"
#include "PublisherStatic.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

void GameScene::initialize()
{
    panels_.init();
    camera_.init(panels_.map().center());
    renderer_.init();

    snx::Publisher::addSubscriber(
        Event::windowResized,
        [&]()
        { panels_.init(); });

    snx::Publisher::addSubscriber(
        Event::panelsResized,
        [&]()
        { camera_.init(panels_.map().center()); });
}

void GameScene::processInput()
{
    // Mouse input
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        cursor_.toggle();
    }

    if (cursor_.isActive())
    {
        cursor_.update(camera_.get());
    }
}

void GameScene::updateState()
{
    if (IsWindowResized())
    {
        snx::Publisher::notify(Event::windowResized);
    }
}

void GameScene::renderOutput()
{
    // Draw panel contents
    BeginMode2D(camera_.get());
    BeginScissorMode(
        panels_.map().left(),
        panels_.map().top(),
        panels_.map().width(),
        panels_.map().height());

    // Draw cursor
    if (cursor_.isActive())
    {
        renderer_.render(
            cursor_.renderID(),
            cursor_.position().renderPosition());
    }

    // Draw world
    // Draw hero
    renderer_.render(
        world_.hero().renderID(),
        world_.hero().position().renderPosition());

    EndScissorMode();
    EndMode2D();

    // Draw panel borders
    DrawRectangleLinesEx(
        panels_.tileInfo().rectangle(),
        PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        panels_.info().rectangle(),
        PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        panels_.status().rectangle(),
        PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        panels_.log().rectangle(),
        PANEL_BORDER_WEIGHT,
        DARKGRAY);

    DrawRectangleLinesEx(
        panels_.map().rectangle(),
        PANEL_BORDER_WEIGHT,
        DARKGRAY);
}

void GameScene::postOutput()
{
}

void GameScene::update()
{

    processInput();
    updateState();

    BeginDrawing();

    ClearBackground(BACKGROUND_COLOR);

    renderOutput();

    // Draw simple frame
    drawWindowBorder();

    if (DeveloperMode::isActive())
    {
        DrawFPS(0, 0);
    }

    EndDrawing();

    postOutput();
}

void GameScene::deinitialize()
{
    renderer_.deinit();
}
