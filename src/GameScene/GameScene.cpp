#include "GameScene.h"

#include "Chunk.h"
#include "ChunksToRender.h"
#include "Colors.h"
#include "Constants/TileData.h"
#include "Cursor.h"
#include "Debugger.h"
#include "DeveloperMode.h"
#include "Enums/RenderID.h"
#include "Event.h"
#include "Fog.h"
#include "GameCamera.h"
#include "Panels.h"
#include "PublisherStatic.h"
#include "Renderer.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "Visibility.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <string>

#ifdef DEBUG
#endif

void GameScene::initialize()
{
    panels_.init();
    gameCamera_.init(panels_.map());
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
            gameCamera_.init(panels_.map());
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
                world_.currentMap(),
                gameCamera_.viewportOnScreen(),
                hero_.position().tilePosition());
        },
        true);

#ifdef DEBUG
    snx::PublisherStatic::addSubscriber(
        Event::cursorPositionChanged,
        [&]()
        {
            Vector2I cursorPos{cursor_.position().tilePosition()};

            snx::debug::cliLog(
                "\nTag: "
                + world_.currentMap().tag(cursorPos)
                + "\n");

            snx::debug::cliLog(
                "TilePosition: "
                + std::to_string(cursorPos.x)
                + ", "
                + std::to_string(cursorPos.y)
                + "\n");

            snx::debug::cliLog(
                "WorldPosition: "
                + std::to_string(world_.currentMap().position(cursorPos).worldPosition().x)
                + ", "
                + std::to_string(world_.currentMap().position(cursorPos).worldPosition().y)
                + "\n");

            snx::debug::cliLog(
                "RenderID: "
                + std::to_string(int(world_.currentMap().renderID(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "VisibilityID: "
                + std::to_string(int(world_.currentMap().visibilityID(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "IsSolid: "
                + std::to_string(world_.currentMap().isSolid(cursorPos))
                + "\n");

            snx::debug::cliLog(
                "IsOpaque: "
                + std::to_string(world_.currentMap().isOpaque(cursorPos))
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
    hero_.movement().update(
        hero_.position(),
        hero_.energy());

#ifdef DEBUG
    snx::debug::cam() = gameCamera_.camera();
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
    for (Chunk& chunk : chunksToRender_.chunks())
    {
        renderer_.renderChunk(chunk);
    }

    // Visibility
    for (Fog const& fog : visibility_.fogsToRender())
    {
        Color tint{};

#ifdef DEBUG
        if (fog.isFogOpaque())
        {
            tint = ColorAlpha(RED, 0.5f);
        }
        else
        {
            tint = ColorAlpha(BLUE, 0.5f);
        }

        DrawRectangleV(
            Vector2SubtractValue(
                UnitConversion::tileToWorld(fog.tilePosition()),
                TileData::TILE_SIZE_HALF),
            TileData::TILE_DIMENSIONS,
            tint);
#else
        if (fog.isFogOpaque())
        {
            tint = BLACK;
        }
        else
        {
            tint = ColorAlpha(BLACK, 0.5f);
        }

        DrawRectangleV(
            Vector2SubtractValue(
                UnitConversion::tileToWorld(fog.tilePosition()),
                TileData::TILE_SIZE_HALF),
            TileData::TILE_DIMENSIONS,
            tint);
#endif
    }

    /*
    Tiles& currentMap{world_.currentMap()};
    Vector2I viewportBottomRight{camera_.viewportInTiles().bottomRight()};

    for (int x{gameCamera_.viewportInTiles().topLeft().x}; x <= viewportBottomRight.x; ++x)
    {
        for (int y{gameCamera_.viewportOnScreen()->topLeft().y}; y <= viewportBottomRight.y; ++y)
        {
            Vector2I tilePosition{x, y};
            switch (currentMap.visibilityID(tilePosition))
            {
            case VisibilityID::invisible:
                if (DeveloperMode::isActive())
                {
                    DrawRectangleV(
                        Vector2SubtractValue(
                            currentMap.position(tilePosition).worldPosition(),
                            TileData::TILE_SIZE_HALF),
                        TileData::TILE_DIMENSIONS,
                        ColorAlpha(
                            RED,
                            0.5));
                }
                else
                {
                    DrawRectangleV(
                        Vector2SubtractValue(
                            currentMap.position(tilePosition).worldPosition(),
                            TileData::TILE_SIZE_HALF),
                        TileData::TILE_DIMENSIONS,
                        BLACK);
                }
                break;

            case VisibilityID::seen:
                if (DeveloperMode::isActive())
                {
                    DrawRectangleV(
                        Vector2SubtractValue(
                            currentMap.position(tilePosition).worldPosition(),
                            TileData::TILE_SIZE_HALF),
                        TileData::TILE_DIMENSIONS,
                        ColorAlpha(
                            BLUE,
                            0.5));
                }
                else
                {
                    DrawRectangleV(
                        Vector2SubtractValue(
                            currentMap.position(tilePosition).worldPosition(),
                            TileData::TILE_SIZE_HALF),
                        TileData::TILE_DIMENSIONS,
                        ColorAlpha(
                            BLACK,
                            0.5));
                }
                break;

            default:
            case VisibilityID::visible:
                break;
            }
        }
    }
    */

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
