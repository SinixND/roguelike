#include "SceneMain.h"
// #define DEBUG_TILEINFO
//* #define DEBUG_FOG

#include "Chunk.h"
#include "Colors.h"
#include "Cursor.h"
#include "Debugger.h"
#include "DeveloperMode.h"
#include "Enemies.h"
#include "Event.h"
#include "Game.h"
#include "GameCamera.h"
#include "Objects.h"
#include "Panels.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "RenderSystem.h"
#include "VisibilitySystem.h"
#include "raylibEx.h"
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

#if defined(DEBUG) && defined(DEBUG_TILEINFO)
#include "Objects.h"
#include <string>
#endif

void SceneMain::init()
{
    game_.init();

    panels_.init();

    gameCamera_.init(
        panels_.map,
        game_.hero.position.worldPixel());

#if defined(DEBUG)
    snx::debug::gcam() = gameCamera_;
#endif

    renderer_.init();

    tileChunks_.init(
        game_.world.currentMap->tiles.positions,
        game_.world.currentMap->tiles.renderIDs,
        renderer_);

    //* Setup events
    setupSceneEvents();
}

void SceneMain::setupSceneEvents()
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
            gameCamera_.init(panels_.map, game_.hero.position.worldPixel());
            visibility_.update(
                // game_.world.currentMap->tiles,
                game_.world.currentMap->tiles.visibilityIDs,
                game_.world.currentMap->tiles.isOpaques,
                gameCamera_.viewportInTiles(),
                game_.hero.position.tilePosition());
        });

    snx::PublisherStatic::addSubscriber(
        Event::heroMoved,
        [&]()
        {
            gameCamera_.setTarget(game_.hero.position.worldPixel());
        });

    snx::PublisherStatic::addSubscriber(
        Event::heroPositionChanged,
        [&]()
        {
            //* VisibilitySystem
            visibility_.update(
                // game_.world.currentMap->tiles,
                game_.world.currentMap->tiles.visibilityIDs,
                game_.world.currentMap->tiles.isOpaques,
                gameCamera_.viewportInTiles(),
                game_.hero.position.tilePosition());
        },
        true);

    snx::PublisherStatic::addSubscriber(
        Event::mapChange,
        [&]()
        {
            tileChunks_.init(
                game_.world.currentMap->tiles.positions,
                game_.world.currentMap->tiles.renderIDs,
                renderer_);
        });

    snx::PublisherStatic::addSubscriber(
        Event::colorThemeChange,
        [&]()
        {
            renderer_.cycleThemes();
            renderer_.init();

            snx::PublisherStatic::publish(Event::mapChange);
        });

    snx::PublisherStatic::addSubscriber(
        Event::cursorToggle,
        [&]()
        {
            cursor_.toggle();
        });

#if defined(DEBUG) && defined(DEBUG_TILEINFO)
    snx::PublisherStatic::addSubscriber(
        Event::cursorPositionChanged,
        [&]()
        {
            Vector2I cursorPos{cursor_.position.tilePosition()};

            if (!game_.world.currentMap->tiles.positions_.contains(cursorPos))
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
                + std::to_string(game_.world.currentMap->tiles.positions_.at(cursorPos).worldPixel().x)
                + ", "
                + std::to_string(game_.world.currentMap->tiles.positions_.at(cursorPos).worldPixel().y)
                + "\n");

            snx::debug::cliLog(
                "RenderID: "
                + std::to_string(static_cast<int>(game_.world.currentMap->tiles.renderIDs_.at(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "VisibilityID: "
                + std::to_string(static_cast<int>(game_.world.currentMap->tiles.visibilityIDs_.at(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "IsSolid: "
                + std::to_string(game_.world.currentMap->tiles.isSolid(cursorPos))
                + "\n");

            snx::debug::cliLog(
                "IsOpaque: "
                + std::to_string(game_.world.currentMap->tiles.isOpaque(cursorPos))
                + "\n");

            if (game_.world.currentMap->objects.positions_.contains(cursorPos))
            {
                snx::debug::cliLog("OBJECT\n");

                snx::debug::cliLog(
                    "\nName: "
                    + game_.world.currentMap->objects.names.at(cursorPos)
                    + "\n");

                snx::debug::cliLog(
                    "\nActions: "
                    + game_.world.currentMap->objects.actions.at(cursorPos)
                    + "\n");

                snx::debug::cliLog(
                    "RenderID: "
                    + std::to_string(static_cast<int>(game_.world.currentMap->objects.renderIDs_.at(cursorPos)))
                    + "\n");

                snx::debug::cliLog(
                    "Event: "
                    + std::to_string(static_cast<int>(game_.world.currentMap->objects.events_.at(cursorPos)))
                    + "\n");
            }

            if (game_.world.currentMap->enemies.ids_.contains(cursorPos))
            {
                snx::debug::cliLog("OBJECT\n");
            }
        });
#endif
}

void SceneMain::processInput()
{
    //* Color theme
    if (IsKeyPressed(KEY_F2))
    {
        snx::PublisherStatic::publish(Event::colorThemeChange);
    }

    game_.processInput(cursor_);
}

void SceneMain::updateState()
{
    cursor_.update(
        gameCamera_.camera(),
        game_.hero.position.tilePosition());

    game_.updateState(
        gameCamera_,
        cursor_);

#if defined(DEBUG)
    snx::debug::gcam() = gameCamera_;
#endif
}

void SceneMain::renderOutput()
{
    //* Draw map panel content
    BeginMode2D(gameCamera_.camera());
    BeginScissorMode(
        gameCamera_.viewportOnScreen().left(),
        gameCamera_.viewportOnScreen().top(),
        gameCamera_.viewportOnScreen().width(),
        gameCamera_.viewportOnScreen().height());

    //* World
    //* Draw map
    //* Draw tiles
    for (Chunk const& chunk : tileChunks_.chunks.values())
    {
        renderer_.renderChunk(chunk);
    }

    //* Draw objects
    Objects const& objects{game_.world.currentMap->objects};

    for (PositionComponent const& position : objects.positions.values())
    {
        renderer_.render(
            objects.renderIDs.at(position.tilePosition()),
            position.worldPixel());
    }

    //* Draw enemies
    Enemies const& enemies{game_.world.currentMap->enemies};

    for (size_t id : enemies.ids)
    {
        renderer_.render(
            enemies.renderIDs.at(id),
            enemies.positions.at(id).worldPixel());
    }

    //* VisibilitySystem
    for (Fog const& fog : visibility_.fogs())
    {
        renderer_.renderFog(fog);
    }

    //* Units
    //* Draw hero
    renderer_.render(
        game_.hero.renderID,
        game_.hero.position.worldPixel());

    //* UI
    //* Draw cursor
    if (cursor_.isActive)
    {
        renderer_.render(
            cursor_.renderID,
            cursor_.position.worldPixel());
    }

    EndScissorMode();
    EndMode2D();

    panels_.drawLogPanelContent();

    panels_.drawHeroInfoPanelContent(game_.hero);

    panels_.drawTileInfoPanelContent(
        game_.world.currentMap->objects,
        cursor_.position.tilePosition());

    panels_.drawGameInfoPanelContent(game_.world.currentMapLevel);

    panels_.drawPanelBorders();
}

void SceneMain::update()
{
    processInput();
    updateState();

    BeginDrawing();

    ClearBackground(BG_COLOR);

    renderOutput();

    //* Draw simple frame
    drawWindowBorder();

    if (DeveloperMode::isActive())
    {
        DrawFPS(0, 0);
    }

    EndDrawing();
}

void SceneMain::deinitialize()
{
    renderer_.deinit();
}
