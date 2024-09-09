#include "GameScene.h"
#define DEBUG_TILEINFO
// #define DEBUG_FOG

#include "Chunk.h"
#include "Chunks.h"
#include "Colors.h"
#include "Cursor.h"
#include "Debugger.h"
#include "DeveloperMode.h"
#include "Event.h"
#include "GameCamera.h"
#include "Logger.h"
#include "Panels.h"
#include "Position.h"
#include "PublisherStatic.h"
#include "Renderer.h"
#include "Visibility.h"
#include "raylibEx.h"
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

#ifdef DEBUG
#include "RNG.h"
#endif

#if defined(DEBUG) && defined(DEBUG_TILEINFO)
#include "Objects.h"
#include <string>
#endif

void GameScene::initialize()
{
#ifdef DEBUG
    snx::RNG::seed(1);
#endif

    panels_.init();

    hero_.init();

    gameCamera_.init(
        panels_.map(),
        hero_.position().worldPosition());

#ifdef DEBUG
    snx::debug::gcam() = gameCamera_;
#endif

    renderer_.init();

    inputHandler_.setDefaultInputMappings();

    tileChunks_.init(world_.currentMap().tiles(), renderer_);

    // Setup events
    setupEvents();

#ifdef DEBUG
    snx::PublisherStatic::publish(Event::nextLevel);
#endif
}

void GameScene::setupEvents()
{
    snx::PublisherStatic::addSubscriber(
        Event::windowResized,
        [&]()
        {
            panels_.init();
        },
        true);

    snx::PublisherStatic::addSubscriber(
        Event::panelsResized,
        [&]()
        {
            gameCamera_.init(panels_.map(), hero_.position().worldPosition());
            visibility_.update(
                world_.currentMap().tiles(),
                gameCamera_.viewportInTiles(),
                hero_.position().tilePosition());
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
        Event::heroMoved,
        [&]()
        {
            gameCamera_.setTarget(hero_.position().worldPosition());
        });

    snx::PublisherStatic::addSubscriber(
        Event::heroPositionChanged,
        [&]()
        {
            // Visibility
            visibility_.update(
                world_.currentMap().tiles(),
                gameCamera_.viewportInTiles(),
                hero_.position().tilePosition());
        },
        true);

    snx::PublisherStatic::addSubscriber(
        Event::nextLevel,
        [&]()
        {
            snx::Logger::log("Enter next Level...");

            world_.increaseMapLevel();

            hero_.position().changeTo(Vector2I{0, 0});

            tileChunks_.init(world_.currentMap().tiles(), renderer_);

            snx::PublisherStatic::publish(Event::heroMoved);
            snx::PublisherStatic::publish(Event::heroPositionChanged);
        });

#if defined(DEBUG) && defined(DEBUG_TILEINFO)
    snx::PublisherStatic::addSubscriber(
        Event::cursorPositionChanged,
        [&]()
        {
            Vector2I cursorPos{cursor_.position().tilePosition()};

            if (!world_.currentMap().tiles().positions().contains(cursorPos))
            {
                return;
            }

            snx::debug::cliPrint("\n");
            snx::debug::cliLog("TILE:\n");

            snx::debug::cliLog(
                "TilePosition: "
                + std::to_string(cursorPos.x)
                + ", "
                + std::to_string(cursorPos.y)
                + "\n");

            snx::debug::cliLog(
                "WorldPosition: "
                + std::to_string(world_.currentMap().tiles().position(cursorPos).worldPosition().x)
                + ", "
                + std::to_string(world_.currentMap().tiles().position(cursorPos).worldPosition().y)
                + "\n");

            snx::debug::cliLog(
                "RenderID: "
                + std::to_string(int(world_.currentMap().tiles().renderID(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "VisibilityID: "
                + std::to_string(int(world_.currentMap().tiles().visibilityID(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "IsSolid: "
                + std::to_string(world_.currentMap().tiles().isSolid(cursorPos))
                + "\n");

            snx::debug::cliLog(
                "IsOpaque: "
                + std::to_string(world_.currentMap().tiles().isOpaque(cursorPos))
                + "\n");

            if (!world_.currentMap().objects().positions().contains(cursorPos))
            {
                return;
            }

            snx::debug::cliLog("OBJECT\n");

            snx::debug::cliLog(
                "\nTag: "
                + world_.currentMap().objects().tag(cursorPos)
                + "\n");

            snx::debug::cliLog(
                "RenderID: "
                + std::to_string(int(world_.currentMap().objects().renderID(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "Event: "
                + std::to_string(int(world_.currentMap().objects().event(cursorPos)))
                + "\n");
        });
#endif
}

void GameScene::processInput()
{
    // Block input handling if hero misses energy
    if (!hero_.energy().isFull())
    {
        return;
    }

    // Take input from mouse, keys and gestures
    inputHandler_.takeInput(cursor_.isActive());
}

void GameScene::updateState()
{
    cursor_.update(
        gameCamera_.camera(),
        hero_.position().worldPosition());

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

    inputHandler_.triggerAction(
        hero_,
        cursor_,
        world_.currentMap(),
        gameCamera_);

    // Check collision before starting movement
    if (hero_.movement().isTriggered())
    {
        if (world_.currentMap().tiles().isSolid(
                // Next tilePosition hero moves to
                Vector2Add(
                    hero_.position().tilePosition(),
                    hero_.movement().direction())))
        {
            hero_.movement().abortMovement();
        }
    }

    // Update hero movment
    hero_.movement().update(
        hero_.position(),
        hero_.energy());

#ifdef DEBUG
    snx::debug::gcam() = gameCamera_;
#endif
}

void GameScene::renderOutput()
{
    // Draw map panel content
    BeginMode2D(gameCamera_.camera());
    BeginScissorMode(
        gameCamera_.viewportOnScreen().left(),
        gameCamera_.viewportOnScreen().top(),
        gameCamera_.viewportOnScreen().width(),
        gameCamera_.viewportOnScreen().height());

    // World
    // Draw map
    // Draw tiles
    for (Chunk& chunk : tileChunks_.chunks().values())
    {
        renderer_.renderChunk(chunk);
    }

    // Draw objects
    auto& objects{world_.currentMap().objects()};
    for (Position const& position : objects.positions().values())
    {
        renderer_.render(
            objects.renderID(position.tilePosition()),
            position.worldPosition());
    }

    // Draw enemies
    auto& enemies{world_.currentMap().enemies()};
    for (size_t id : enemies.ids().values())
    {
        renderer_.render(
            enemies.renderID(id),
            enemies.position(id).worldPosition());
    }

    // Visibility
    for (Fog const& fog : visibility_.fogsToRender())
    {
        renderer_.renderFog(fog);
    }

    // Units
    // Draw hero
    renderer_.render(
        hero_.renderID(),
        hero_.position().worldPosition());

    // UI
    // Draw cursor
    if (cursor_.isActive())
    {
        renderer_.render(
            cursor_.renderID(),
            cursor_.position().worldPosition());
    }

    EndScissorMode();
    EndMode2D();

    panels_.drawLogPanelContent();
    panels_.drawTileInfoPanelContent(world_.currentMap().objects(), cursor_.position().tilePosition());

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
