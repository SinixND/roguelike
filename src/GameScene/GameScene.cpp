#include "GameScene.h"

#include "Cursor.h"
#include "DeveloperMode.h"
#include "Event.h"
#include "PublisherStatic.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

void GameScene::initialize()
{
    panels_.init();
    camera_.init(panels_.map().center());
    renderer_.init();

    inputHandler_.setDefaultInputMappings();

    hero_.init();

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

    snx::Publisher::addSubscriber(
        Event::cameraChanged,
        [&]()
        { initTilesToRender(
); },
        true);

    snx::Publisher::addSubscriber(
        Event::panelsResized,
        [&]()
        { initTilesToRender(
); });
}

void GameScene::processInput()
{
    // Mouse input
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        cursor_.toggle();
    }

    cursor_.update(camera_.get(), hero_.position().renderPosition());

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
    BeginMode2D(camera_.get());
    BeginScissorMode(
        panels_.map().left(),
        panels_.map().top(),
        panels_.map().width(),
        panels_.map().height());

    // World
    // Draw map
    for (size_t i{0}; i < tilesToRender_.renderPositions.size(); ++i)
    {
        renderer_.render(
           tilesToRender_.renderIDs[i],
            tilesToRender_.renderPositions[i]);
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

RectangleExI getRenderRectangle(
    GameCamera const& gameCamera,
    RectangleEx const& mapPanel)
{
    return RectangleExI{
        Vector2SubtractValue(
            UnitConversion::screenToTilePosition(
                mapPanel.topLeft(),
                gameCamera.get()),
            1),
        Vector2AddValue(
            UnitConversion::screenToTilePosition(
                mapPanel.bottomRight(),
                gameCamera.get()),
            1)};
}

std::vector<Vector2I> GameScene::tilePositionsToRender(
    GameCamera const& gameCamera,
    RectangleEx const& mapPanel)
{
    RectangleExI renderRectangle{getRenderRectangle(gameCamera, mapPanel)};
    Vector2I tilePosition{renderRectangle.topLeft()};
    std::vector<Vector2I> tilePositions{};

    for (int x{0}; x < renderRectangle.width(); ++x)
    {
        for (int y{0}; y < renderRectangle.height(); ++y)
        {
            Vector2I position{tilePosition.x + x, tilePosition.y + y};

            if (
                !visibilityIDs_.contains(position)
                || (visibilityIDs_[position] == VisibilityID::invisible))
            {
                continue;
            }

            tilePositions.push_back(position);
        }
    }

    return tilePositions;
};

void GameScene::initTilesToRender(
    GameCamera const& gameCamera,
    RectangleEx const& mapPanel)
{
    std::vector<Vector2I> tilePositions{tilePositionsToRender(
            gameCamera,
            mapPanel)};

    for (auto& tilePosition : tilePositions)
    {
        tilesToRender_.renderPositions.push_back(
            positions_[tilePosition].renderPosition());

        tilesToRender_.renderIDs.push_back(
            renderIDs_[tilePosition]);

        tilesToRender_.visibilityIDs.push_back(
            visibilityIDs_[tilePosition]);
    }
};

// void GameScene::updateTilesToRender(std::vector<Vector2I>  const& tilePositions)
// {
//
// }
