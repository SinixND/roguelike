#include "GameScene.h"

#include "Colors.h"
#include "Cursor.h"
#include "DeveloperMode.h"
#include "Event.h"
#include "GameCamera.h"
#include "Panels.h"
#include "PublisherStatic.h"
#include "Renderer.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
#include <iostream>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>
#include <vector>

void GameScene::initialize()
{
    panels_.init();
    gameCamera_.init(panels_.map().center());
    renderer_.init();

    inputHandler_.setDefaultInputMappings();

    hero_.init();

    // Setup events
    // App::eventDispatcher.addSubscriber(
    snx::PublisherStatic::addSubscriber(
        Event::windowResized,
        [&]()
        { panels_.init(); });

    // eventDispatcher.addSubscriber(
    snx::PublisherStatic::addSubscriber(
        Event::panelsResized,
        [&]()
        { gameCamera_.init(panels_.map().center()); });

    // eventDispatcher.addSubscriber(
    snx::PublisherStatic::addSubscriber(
        Event::actionInProgress,
        [&]()
        { actionInProgress_ = true; });

    // eventDispatcher.addSubscriber(
    snx::PublisherStatic::addSubscriber(
        Event::actionFinished,
        [&]()
        { actionInProgress_ = false; });

    // eventDispatcher.addSubscriber(
    snx::PublisherStatic::addSubscriber(
        Event::cameraChanged,
        [&]()
        { initTilesToRender(); },
        true);

    // eventDispatcher.addSubscriber(
    snx::PublisherStatic::addSubscriber(
        Event::panelsResized,
        [&]()
        { initTilesToRender(); });

    // eventDispatcher.addSubscriber(
    snx::PublisherStatic::addSubscriber(
        Event::positionChanged,
        [&]()
        { gameCamera_.setTarget(hero_.position().renderPosition()); });
}

void GameScene::processInput()
{
    // Mouse input
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        cursor_.toggle();
    }

    cursor_.update(gameCamera_.get(), hero_.position().renderPosition());

    inputHandler_.handleInput();

    // Block input handling if hero misses energy
    if (!hero_.energy().isFull())
    {
        return;
    }

    inputHandler_.update(hero_);
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

    // Update hero movment
    hero_.movement().update(hero_.position());
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
    // Iterate whole map (1) or filtered list (0)
    if (0)
    {
        // snx::BM::start("Iterate Map");
        auto& map{world_.currentMap()};
        auto renderRectangle{renderRectangleExI()};
        auto& renderIDs{map.renderIDs().values()};
        auto& positions{map.positions().values()};
        auto& visibilityIDs{map.visibilityIDs().values()};

        std::cout << "Map size: " << map.size() << "\n";
        for (size_t i{0}; i < map.size(); ++i)
        {
            // snx::BM::start("Map loop");
            if (
                !CheckCollisionPointRec(positions[i].tilePosition(), renderRectangle)
                // || map.visibilityIDs().values()[i] == VisibilityID::invisible
            )
            {
                continue;
            }

            Color tint{WHITE};

            // Set tint alpha per visibility
            switch (visibilityIDs[i])
            {
            case VisibilityID::visible:
                tint = ColorAlpha(tint, 1.0);
                break;
            case VisibilityID::seen:
                tint = ColorAlpha(tint, 0.5);
                break;
            default:
                break;
            }

            // To Debug:
            tint = ColorAlpha(tint, 1.0);

            renderer_.render(
                renderIDs[i],
                positions[i].renderPosition(),
                tint);
            // snx::BM::stop("Map loop");
        }
        // snx::BM::stop("Iterate Map");
        // snx::BM::report("Iterate Map");
        // snx::BM::report("Map loop");
    }
    else
    {
        // snx::BM::start("Iterate Filtered");
        size_t tileCount{tilesToRender_.renderPositions().size()};
        auto const& renderIDs{tilesToRender_.renderIDs()};
        auto const& renderPositions{tilesToRender_.renderPositions()};
        auto const& visibilityIDs{tilesToRender_.visibilityIDs()};

        std::cout << "Tile count: " << tileCount << "\n";
        for (size_t i{0}; i < tileCount; ++i)
        {
            // snx::BM::start("Filtered loop");
            Color tint{WHITE};

            // Set tint alpha per visibility
            switch (visibilityIDs[i])
            {
            case VisibilityID::visible:
                tint = ColorAlpha(tint, 1.0);
                break;
            case VisibilityID::seen:
                tint = ColorAlpha(tint, 0.5);
                break;
            default:
                break;
            }

            // For debug
            tint = ColorAlpha(tint, 1.0);

            renderer_.render(
                renderIDs[i],
                renderPositions[i],
                tint);
            // snx::BM::stop("Filtered loop");
        }
        // snx::BM::stop("Iterate Filtered");
        // snx::BM::report("Iterate Filtered");
        // snx::BM::report("Filtered loop");
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

RectangleExI GameScene::renderRectangleExI()
{
    return RectangleExI{
        Vector2SubtractValue(
            UnitConversion::screenToTile(
                panels_.map().topLeft(),
                gameCamera_.get()),
            1),
        Vector2AddValue(
            UnitConversion::screenToTile(
                panels_.map().bottomRight(),
                gameCamera_.get()),
            1)};
}

std::vector<Vector2I> GameScene::tilePositionsToRender()
{
    Tiles& currentMap{world_.currentMap()};
    RectangleExI renderRect{renderRectangleExI()};
    Vector2I initialTilePositionToCheck{renderRect.topLeft()};
    std::vector<Vector2I> tilePositionsToReturn{};

    for (int x{0}; x < renderRect.width(); ++x)
    {
        for (int y{0}; y < renderRect.height(); ++y)
        {
            Vector2I tilePositionToCheck{initialTilePositionToCheck.x + x, initialTilePositionToCheck.y + y};

            if (
                !currentMap.visibilityIDs().contains(tilePositionToCheck)
                // || (currentMap.visibilityIDs()[tilePositionToCheck] == VisibilityID::invisible)
            )
            {
                continue;
            }

            tilePositionsToReturn.push_back(tilePositionToCheck);
        }
    }

    return tilePositionsToReturn;
};

void GameScene::initTilesToRender()
{
    tilesToRender_.clear();
    std::vector<Vector2I> tilePositions{tilePositionsToRender()};
    Tiles& currentMap{world_.currentMap()};

    for (auto& tilePosition : tilePositions)
    {
        tilesToRender_.insert(currentMap, tilePosition);
    }
};

// void GameScene::updateTilesToRender(std::vector<Vector2I>  const& tilePositions)
// {
//
// }
