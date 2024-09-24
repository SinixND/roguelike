#include "GameScene.h"
#include <cmath>
// #define DEBUG_TILEINFO
// #define DEBUG_FOG

#include "Chunk.h"
#include "Chunks.h"
#include "Collision.h"
#include "Colors.h"
#include "Cursor.h"
#include "Debugger.h"
#include "DeveloperMode.h"
#include "Enemies.h"
#include "Event.h"
#include "GameCamera.h"
#include "Logger.h"
#include "Objects.h"
#include "Panels.h"
#include "Position.h"
#include "PublisherStatic.h"
#include "RenderID.h"
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
        hero_.position_.worldPixel());

#ifdef DEBUG
    snx::debug::gcam() = gameCamera_;
#endif

    renderer_.init();

    inputHandler_.setDefaultInputMappings();

    tileChunks_.init(world_.currentMap().tiles_, renderer_);

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
            gameCamera_.init(panels_.map(), hero_.position_.worldPixel());
            visibility_.update(
                world_.currentMap().tiles_,
                gameCamera_.viewportInTiles(),
                hero_.position_.tilePosition());
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
            gameCamera_.setTarget(hero_.position_.worldPixel());
        });

    snx::PublisherStatic::addSubscriber(
        Event::heroPositionChanged,
        [&]()
        {
            // Visibility
            visibility_.update(
                world_.currentMap().tiles_,
                gameCamera_.viewportInTiles(),
                hero_.position_.tilePosition());
        },
        true);

    snx::PublisherStatic::addSubscriber(
        Event::nextLevel,
        [&]()
        {
            snx::Logger::log("Enter next Level...");

            world_.increaseMapLevel();

            hero_.position_.changeTo(Vector2I{0, 0});

            tileChunks_.init(world_.currentMap().tiles_, renderer_);

            snx::PublisherStatic::publish(Event::heroMoved);
            snx::PublisherStatic::publish(Event::heroPositionChanged);
        });

    snx::PublisherStatic::addSubscriber(
        Event::previousLevel,
        [&]()
        {
            snx::Logger::log("Enter previous Level...");

            world_.decreaseMapLevel();

            for (Position const& position : world_.currentMap().objects_.positions().values())
            {
                if (!(world_.currentMap().objects_.renderID(position.tilePosition()) == RenderID::descend))
                {
                    continue;
                }

                hero_.position_.changeTo(position.tilePosition());
            }

            tileChunks_.init(world_.currentMap().tiles_, renderer_);

            snx::PublisherStatic::publish(Event::heroMoved);
            snx::PublisherStatic::publish(Event::heroPositionChanged);
        });

    snx::PublisherStatic::addSubscriber(
        Event::colorThemeChange,
        [&]()
        {
            renderer_.cycleThemes();
            renderer_.init();

            tileChunks_.init(world_.currentMap().tiles_, renderer_);
        });

#if defined(DEBUG) && defined(DEBUG_TILEINFO)
    snx::PublisherStatic::addSubscriber(
        Event::cursorPositionChanged,
        [&]()
        {
            Vector2I cursorPos{cursor_.position_.tilePosition()};

            if (!world_.currentMap().tiles_.positions().contains(cursorPos))
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
                "WorldPixel: "
                + std::to_string(world_.currentMap().tiles_.position(cursorPos).worldPixel().x)
                + ", "
                + std::to_string(world_.currentMap().tiles_.position(cursorPos).worldPixel().y)
                + "\n");

            snx::debug::cliLog(
                "RenderID: "
                + std::to_string(int(world_.currentMap().tiles_.renderID(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "VisibilityID: "
                + std::to_string(int(world_.currentMap().tiles_.visibilityID(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "IsSolid: "
                + std::to_string(world_.currentMap().tiles_.isSolid(cursorPos))
                + "\n");

            snx::debug::cliLog(
                "IsOpaque: "
                + std::to_string(world_.currentMap().tiles_.isOpaque(cursorPos))
                + "\n");

            if (!world_.currentMap().objects_.positions().contains(cursorPos))
            {
                return;
            }

            snx::debug::cliLog("OBJECT\n");

            snx::debug::cliLog(
                "\nTag: "
                + world_.currentMap().objects_.tag(cursorPos)
                + "\n");

            snx::debug::cliLog(
                "RenderID: "
                + std::to_string(int(world_.currentMap().objects_.renderID(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "Event: "
                + std::to_string(int(world_.currentMap().objects_.event(cursorPos)))
                + "\n");
        });
#endif
}

void GameScene::processInput()
{
    // Color theme
    if (IsKeyPressed(KEY_F2))
    {
        snx::PublisherStatic::publish(Event::colorThemeChange);
    }

    // Block input handling if hero misses energy
    if (!hero_.energy_.isFull())
    {
        return;
    }

    // Take input from mouse, keys or gestures
    inputHandler_.takeInput(cursor_.isActive());
}

void GameScene::updateState()
{
    cursor_.update(
        gameCamera_.camera(),
        hero_.position_.tilePosition());

    // Regenerate energy if no action in progress
    if (!actionInProgress_)
    {
        while (true)
        {
            if (hero_.energy_.regenerate())
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

    // Avoid check if no movement in progress
    if (hero_.movement_.isTriggered())
    {
        // Check collision before starting movement
        if (Collision::checkCollision(
                world_.currentMap(),
                Vector2Add(
                    hero_.position_.tilePosition(),
                    hero_.movement_.direction())))
        {
            hero_.movement_.abortMovement();
        }
    }

    // Update hero movment
    hero_.movement_.update(
        hero_.position_,
        hero_.energy_);

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
    for (Chunk const& chunk : tileChunks_.chunks().values())
    {
        renderer_.renderChunk(chunk);
    }

    // Draw objects
    Objects const& objects{world_.currentMap().objects_};

    for (Position const& position : objects.positions().values())
    {
        renderer_.render(
            objects.renderID(position.tilePosition()),
            position.worldPixel());
    }

    // Draw enemies
    Enemies const& enemies{world_.currentMap().enemies_};

    for (size_t id : enemies.ids().values())
    {
        renderer_.render(
            enemies.renderID(id),
            enemies.position(id).worldPixel());
    }

    // Visibility
    for (Fog const& fog : visibility_.fogs())
    {
        renderer_.renderFog(fog);
    }

    // Units
    // Draw hero
    renderer_.render(
        hero_.renderID(),
        hero_.position_.worldPixel());

    // UI
    // Draw cursor
    if (cursor_.isActive())
    {
        renderer_.render(
            cursor_.renderID(),
            cursor_.position_.worldPixel());
    }

    EndScissorMode();
    EndMode2D();

    panels_.drawLogPanelContent();
    panels_.drawTileInfoPanelContent(world_.currentMap().objects_, cursor_.position_.tilePosition());
    panels_.drawStatusPanelContent(world_.currentMapLevel());

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
