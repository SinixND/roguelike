#include "GameScene.h"

#include "Constants/Colors.h"
#include "Cursor.h"
#include "DeveloperMode.h"
#include "Event.h"
#include "GameCamera.h"
#include "Panels.h"
#include "PublisherStatic.h"
#include "Renderer.h"
#include "TileData.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <cstddef>
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

    initRenderTextureMap();

    // Setup events
    snx::Publisher::addSubscriber(
        Event::windowResized,
        [&]()
        { panels_.init(); });

    snx::Publisher::addSubscriber(
        Event::panelsResized,
        [&]()
        { gameCamera_.init(panels_.map().center()); });

    snx::Publisher::addSubscriber(
        Event::actionInProgress,
        [&]()
        { actionInProgress_ = true; });

    snx::Publisher::addSubscriber(
        Event::actionFinished,
        [&]()
        { actionInProgress_ = false; });

    snx::Publisher::addSubscriber(
        Event::cameraChanged,
        [&]()
        { initTilesToRender(); },
        true);

    snx::Publisher::addSubscriber(
        Event::panelsResized,
        [&]()
        { initTilesToRender(); });
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
    if (IsWindowResized())
    {
        snx::Publisher::publish(Event::windowResized);
    }

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
    DrawTextureRec(
        renderTextureMap_.texture,
        Rectangle{
            0,
            0,
            static_cast<float>(renderTextureMap_.texture.width),
            static_cast<float>(-renderTextureMap_.texture.height)},
        // Vector2{0, 0},
        Vector2{
            -renderTextureMap_.texture.width / 2.0f,
            -renderTextureMap_.texture.height / 2.0f},
        WHITE);

    // auto& map{world_.currentMap()};

    // for (size_t i{0}; i < map.size(); ++i)
    // {
    //     if (
    //         !CheckCollisionPointRec(map.positions().values()[i].tilePosition(), renderRectangleExI())
    //         // || map.visibilityIDs().values()[i] == VisibilityID::invisible
    //     )
    //     {
    //         continue;
    //     }

    //     Color tint{WHITE};

    //     // Set tint alpha per visibility
    //     switch (tilesToRender_.visibilityIDs()[i])
    //     {
    //     case VisibilityID::visible:
    //         tint = ColorAlpha(tint, 1.0);
    //         break;
    //     case VisibilityID::seen:
    //         tint = ColorAlpha(tint, 0.5);
    //         break;
    //     default:
    //         break;
    //     }

    //     // To Debug:
    //     tint = ColorAlpha(tint, 1.0);

    //             renderer_.render(
    //                 map.renderIDs().values()[i],
    //                 map.positions().values()[i].renderPosition(),
    //                 tint);
    // }

    // for (size_t i{0}; i < tilesToRender_.renderPositions().size(); ++i)
    // {
    //     Color tint{WHITE};

    //     // Set tint alpha per visibility
    //     switch (tilesToRender_.visibilityIDs()[i])
    //     {
    //     case VisibilityID::visible:
    //         tint = ColorAlpha(tint, 1.0);
    //         break;
    //     case VisibilityID::seen:
    //         tint = ColorAlpha(tint, 0.5);
    //         break;
    //     default:
    //         break;
    //     }

    //     tint = ColorAlpha(tint, 1.0);

    //     renderer_.render(
    //         tilesToRender_.renderIDs()[i],
    //         tilesToRender_.renderPositions()[i],
    //         tint);
    // }

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
                || (currentMap.visibilityIDs()[tilePositionToCheck] == VisibilityID::invisible))
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

void GameScene::initRenderTextureMap()
{
    auto& map{world_.currentMap()};

    renderTextureMap_ = LoadRenderTexture(
        (map.mapSize().width() * TileData::TILE_SIZE),
        (map.mapSize().height() * TileData::TILE_SIZE));

    BeginTextureMode(renderTextureMap_);

    ClearBackground(BG_COLOR);

    for (size_t i{0}; i < map.size(); ++i)
    {
        renderer_.render(
            map.renderIDs().values()[i],
            Vector2Add(
                map.positions().values()[i].renderPosition(),
                Vector2{
                    ((map.mapSize().width() * TileData::TILE_SIZE) / 2),
                    ((map.mapSize().height() * TileData::TILE_SIZE) / 2)}));
    }

    EndTextureMode();
}