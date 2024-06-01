#include "GameScene.h"

#include "Cursor.h"
#include "DeveloperMode.h"
#include "Event.h"
#include "PublisherStatic.h"
#include "World.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

void GameScene::initialize()
{
    panels_.init();
    camera_.init(panels_.map().center());
    renderer_.init();

    world_.init();

    // Setup events
    snx::Publisher::addSubscriber(
        Event::windowResized,
        [&]()
        { panels_.init(); });

    snx::Publisher::addSubscriber(
        Event::panelsResized,
        [&]()
        { camera_.init(panels_.map().center()); });

    snx::Publisher::addSubscriber(
        Event::actionInProgress,
        [&]()
        { actionInProgress_ = true; });

    snx::Publisher::addSubscriber(
        Event::actionFinished,
        [&]()
        { actionInProgress_ = false; });
}

void GameScene::processInput()
{
    // Mouse input
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        cursor_.toggle();
    }

    cursor_.update(camera_.get(), world_.hero().position().renderPosition());

    inputHandler_.check();

    // Block input handling if hero misses energy
    if (!world_.hero().energy().isFull())
    {
        return;
    }

    inputHandler_.update(&world_.hero());
}

void GameScene::updateState()
{
    if (IsWindowResized())
    {
        snx::Publisher::notify(Event::windowResized);
    }

    // Regenerate energy if no action in progress
    if (!actionInProgress_)
    {
        while (true)
        {
            if (world_.hero().energy().regenerate())
            {
                break;
            }
        }
    }

    // Update hero movment
    world_.hero().movement().update(world_.hero().position());
}

void GameScene::renderOutput()
{
    // Draw map panel content
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

    panels_.drawLogPanelContent();

    panels_.drawPanelBorders();
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
