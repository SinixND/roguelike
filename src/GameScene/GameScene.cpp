#include "GameScene.h"

#include "Chunk.h"
#include "ChunksToRender.h"
#include "Colors.h"
#include "Cursor.h"
#include "DeveloperMode.h"
#include "Event.h"
#include "GameCamera.h"
#include "Panels.h"
#include "PublisherStatic.h"
#include "Renderer.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

#ifdef DEBUG
#endif

void GameScene::initialize()
{
    panels_.init();
    gameCamera_.init(panels_.map().center());
    renderer_.init();

    inputHandler_.setDefaultInputMappings();

    hero_.init();

    chunksToRender_.init(world_.currentMap(), renderer_);

    // Setup events
    snx::PublisherStatic::addSubscriber(
        Event::windowResized,
        [&]()
        {
            panels_.init();
        });

    snx::PublisherStatic::addSubscriber(
        Event::panelsResized,
        [&]()
        {
            gameCamera_.init(panels_.map().center());
        });

    snx::PublisherStatic::addSubscriber(
        Event::actionInProgress,
        [&]()
        {
            actionInProgress_ = true;
        });

    snx::PublisherStatic::addSubscriber(
        Event::actionFinished,
        [&]()
        {
            actionInProgress_ = false;
        });

    snx::PublisherStatic::addSubscriber(
        Event::heroPositionChanged,
        [&]()
        {
            gameCamera_.setTarget(hero_.position().renderPosition());
        });
}

void GameScene::processInput()
{
    // Mouse input
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        cursor_.toggle();
    }

    cursor_.update(gameCamera_.get(), hero_.position().renderPosition());

    // Block input handling if hero misses energy
    if (!hero_.energy().isFull())
    {
        return;
    }

    // Stores keyboard and gesture input
    inputHandler_.takeInput();
}

void GameScene::updateState()
{
    // Regenerate energy if no action in progress
    if (!actionInProgress_)
    {
        while (true)
        {
            if (hero_.energy().regenerate())
            {
                break;
            }
        }
    }

    inputHandler_.triggerInput(hero_);

    // Check collision before starting movement
    if (hero_.movement().isTriggered())
    {
        if (world_.currentMap().isSolid(
                // Next tilePosition hero moves to
                Vector2Add(
                    hero_.position().tilePosition(),
                    hero_.movement().direction())))
        {
            hero_.movement().abortMovement();
        }
    }

    // Update hero movment
    hero_.movement().update(hero_.position(), hero_.energy());
}

void GameScene::renderOutput()
{
    // Draw map panel content
    BeginMode2D(gameCamera_.get());
    BeginScissorMode(
        panels_.map().left(),
        panels_.map().top(),
        panels_.map().width(),
        panels_.map().height());

    // World
    // Draw map
    for (Chunk& chunk : chunksToRender_.chunks())
    {
        renderer_.renderChunk(chunk);
    }

    // Units
    // Draw hero
    renderer_.render(
        hero_.renderID(),
        hero_.position().renderPosition());

    // UI
    // Draw cursor
    if (cursor_.isActive())
    {
        renderer_.render(
            cursor_.renderID(),
            cursor_.position().renderPosition());
    }

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

    ClearBackground(BG_COLOR);

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

// RectangleExI GameScene::renderRectangleExI()
// {
//     return RectangleExI{
//         Vector2SubtractValue(
//             UnitConversion::screenToTile(
//                 panels_.map().topLeft(),
//                 gameCamera_.get()),
//             1),
//         Vector2AddValue(
//             UnitConversion::screenToTile(
//                 panels_.map().bottomRight(),
//                 gameCamera_.get()),
//             1)};
// }
