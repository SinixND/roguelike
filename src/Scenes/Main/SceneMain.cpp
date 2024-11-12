#include "SceneMain.h"
#include "VisibilityID.h"

#define DEBUG_TILEINFO
//* #define DEBUG_FOG

#include "ChunkSystem.h"
#include "Colors.h"
#include "Cursor.h"
#include "Debugger.h"
#include "DeveloperMode.h"
#include "Enemies.h"
#include "EventID.h"
#include "Game.h"
#include "GameCamera.h"
#include "Objects.h"
#include "PanelSystem.h"
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

    PanelSystem::init(panels_);

    gameCamera_.init(
        panels_.map,
        game_.hero.position.worldPixel());

#if defined(DEBUG)
    snx::debug::gcam() = gameCamera_;
#endif

    RenderSystem::loadRenderData(renderData_);

    ChunkSystem::initializeChunks(
        renderData_.textures,
        chunks_,
        game_.world.currentMap->tiles.positions,
        game_.world.currentMap->tiles.renderIDs);

    //* Setup events
    setupSceneEvents();
}

void SceneMain::setupSceneEvents()
{
    snx::PublisherStatic::addSubscriber(
        EventID::WINDOW_RESIZED,
        [&]()
        {
            PanelSystem::init(panels_);
        },
        true);

    snx::PublisherStatic::addSubscriber(
        EventID::PANELS_RESIZED,
        [&]()
        {
            gameCamera_.init(panels_.map, game_.hero.position.worldPixel());
            VisibilitySystem::update(
                fogs_,
                // game_.world.currentMap->tiles,
                game_.world.currentMap->tiles.visibilityIDs,
                game_.world.currentMap->tiles.isOpaques,
                gameCamera_.viewportInTiles(),
                game_.hero.visionRange,
                game_.hero.position.tilePosition());
        });

    snx::PublisherStatic::addSubscriber(
        EventID::HERO_MOVED,
        [&]()
        {
            gameCamera_.setTarget(game_.hero.position.worldPixel());
        });

    snx::PublisherStatic::addSubscriber(
        EventID::HERO_POSITION_CHANGED,
        [&]()
        {
            //* VisibilitySystem
            VisibilitySystem::update(
                fogs_,
                // game_.world.currentMap->tiles,
                game_.world.currentMap->tiles.visibilityIDs,
                game_.world.currentMap->tiles.isOpaques,
                gameCamera_.viewportInTiles(),
                game_.hero.visionRange,
                game_.hero.position.tilePosition());
        },
        true);

    snx::PublisherStatic::addSubscriber(
        EventID::MAP_CHANGE,
        [&]()
        {
            ChunkSystem::initializeChunks(
                renderData_.textures,
                chunks_,
                game_.world.currentMap->tiles.positions,
                game_.world.currentMap->tiles.renderIDs);
        });

    snx::PublisherStatic::addSubscriber(
        EventID::COLOR_THEME_CHANGE,
        [&]()
        {
            RenderSystem::cycleThemes(renderData_.theme);
            RenderSystem::loadRenderData(renderData_);

            snx::PublisherStatic::publish(EventID::MAP_CHANGE);
        });

    snx::PublisherStatic::addSubscriber(
        EventID::CURSOR_TOGGLE,
        [&]()
        {
            cursor_.toggle();
        });

#if defined(DEBUG) && defined(DEBUG_TILEINFO)
    snx::PublisherStatic::addSubscriber(
        EventID::CURSOR_POSITION_CHANGED,
        [&]()
        {
            Vector2I cursorPos{cursor_.position.tilePosition()};

            if (!game_.world.currentMap->tiles.positions.contains(cursorPos))
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
                + std::to_string(game_.world.currentMap->tiles.positions.at(cursorPos).worldPixel().x)
                + ", "
                + std::to_string(game_.world.currentMap->tiles.positions.at(cursorPos).worldPixel().y)
                + "\n");

            snx::debug::cliLog(
                "RenderID: "
                + std::to_string(static_cast<int>(game_.world.currentMap->tiles.renderIDs.at(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "VisibilityID: "
                + std::to_string(static_cast<int>(game_.world.currentMap->tiles.visibilityIDs.at(cursorPos)))
                + "\n");

            snx::debug::cliLog(
                "IsSolid: "
                + std::to_string(game_.world.currentMap->tiles.isSolid(cursorPos))
                + "\n");

            snx::debug::cliLog(
                "IsOpaque: "
                + std::to_string(game_.world.currentMap->tiles.isOpaque(cursorPos))
                + "\n");

            if (game_.world.currentMap->objects.positions.contains(cursorPos))
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
                    + std::to_string(static_cast<int>(game_.world.currentMap->objects.renderIDs.at(cursorPos)))
                    + "\n");

                snx::debug::cliLog(
                    "Event: "
                    + std::to_string(static_cast<int>(game_.world.currentMap->objects.events.at(cursorPos)))
                    + "\n");
            }

            if (game_.world.currentMap->enemies.ids.contains(cursorPos))
            {
                snx::debug::cliLog("ENEMY\n");

                snx::debug::cliLog(
                    "ID: "
                    + std::to_string(game_.world.currentMap->enemies.ids.at(cursorPos))
                    + "\n");
            }
        });
#endif
}

void SceneMain::processInput()
{
    //* Color theme
    if (IsKeyPressed(KEY_F2))
    {
        snx::PublisherStatic::publish(EventID::COLOR_THEME_CHANGE);
    }

    //* Allow input if hero is ready (= full energy)
    if (game_.hero.energy.isReady())
    {
        game_.processInput(cursor_);
    }
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
    for (Chunk const& chunk : chunks_)
    {
        RenderSystem::renderChunk(chunk);
    }

    //* Draw objects
    auto const& objects{game_.world.currentMap->objects};
    auto const& objectRenderIDs{objects.renderIDs.values()};
    auto const& objectPositions{objects.positions.values()};

    for (size_t i{0}; i < objectRenderIDs.size(); ++i)
    {
        RenderSystem::render(
            renderData_.textures,
            objectRenderIDs.at(i),
            objectPositions.at(i).worldPixel());
    }

    //* Draw enemies
    auto const& enemies{game_.world.currentMap->enemies};
    auto const& enemyRenderIDs{enemies.renderIDs.values()};
    auto const& enemyPositions{enemies.positions.values()};

    for (size_t i{0}; i < enemyRenderIDs.size(); ++i)
    {
        if (game_.world.currentMap->tiles.visibilityIDs.at(enemyPositions.at(i).tilePosition()) != VisibilityID::VISIBILE)
        {
            continue;
        }

        RenderSystem::render(
            renderData_.textures,
            enemyRenderIDs.at(i),
            enemyPositions.at(i).worldPixel());
    }

    //* VisibilitySystem
    for (Fog const& fog : fogs_)
    {
        RenderSystem::renderFog(fog);
    }

    //* Units
    //* Draw hero
    RenderSystem::render(
        renderData_.textures,
        game_.hero.renderID,
        game_.hero.position.worldPixel());

    //* UI
    //* Draw cursor
    if (cursor_.isActive)
    {
        RenderSystem::render(
            renderData_.textures,
            cursor_.renderID,
            cursor_.position.worldPixel());
    }

    EndScissorMode();
    EndMode2D();

    PanelSystem::drawLogPanelContent(panels_);

    PanelSystem::drawHeroInfoPanelContent(
        panels_,
        game_.hero);

    PanelSystem::drawTileInfoPanelContent(
        panels_,
        game_.world.currentMap->objects,
        cursor_.position.tilePosition());

    PanelSystem::drawGameInfoPanelContent(
        panels_,
        game_.world.currentMapLevel);

    PanelSystem::drawPanelBorders(panels_);
}

void SceneMain::update()
{
    processInput();
    updateState();

    BeginDrawing();

    ClearBackground(bgColor);

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
    RenderSystem::deinit(renderData_.textures);
}
