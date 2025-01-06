#include "SceneMain.h"
#include "Convert.h"
#include "SceneData.h"
#include "VisibilityId.h"

#define DEBUG_TILEINFO
//* #define DEBUG_FOG

#include "ChunkSystem.h"
#include "Colors.h"
#include "Cursor.h"
#include "Debugger.h"
#include "DeveloperMode.h"
#include "Enemies.h"
#include "EventId.h"
#include "Game.h"
#include "GameCamera.h"
#include "Objects.h"
#include "PanelSystem.h"
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

void SceneModule::init(SceneMain& scene)
{
    GameModule::init(scene.game);

    PanelSystem::init(scene.panels);

    GameCameraModule::init(
        scene.gameCamera,
        scene.panels.map,
        scene.game.hero.position);

#if defined(DEBUG)
    snx::debug::gcam() = scene.gameCamera;
#endif

    RenderSystem::loadRenderData(scene.renderData);

    ChunkSystem::init(
        scene.renderData.textures,
        scene.chunks,
        scene.game.world.currentMap->tiles.ids,
        scene.game.world.currentMap->tiles.positions,
        scene.game.world.currentMap->tiles.renderIds);

    //* Setup events
    SceneModule::setupSceneEvents(scene);
}

void SceneModule::setupSceneEvents(SceneMain& scene)
{
    snx::PublisherStatic::addSubscriber(
        EventId::WINDOW_RESIZED,
        [&]()
        {
            PanelSystem::init(scene.panels);
        },
        true);

    snx::PublisherStatic::addSubscriber(
        EventId::PANELS_RESIZED,
        [&]()
        {
            GameCameraModule::init(
                scene.gameCamera,
                scene.panels.map,
                scene.game.hero.position);

            VisibilitySystem::update(
                scene.fogs,
                scene.game.world.currentMap->tiles.visibilityIds,
                scene.game.world.currentMap->tiles.ids,
                scene.game.world.currentMap->tiles.isOpaques,
                GameCameraModule::viewportInTiles(scene.gameCamera),
                scene.game.hero.visionRange,
                Convert::worldToTile(scene.game.hero.position));
        });

    snx::PublisherStatic::addSubscriber(
        EventId::HERO_MOVED,
        [&]()
        {
            GameCameraModule::setTarget(
                scene.gameCamera,
                scene.game.hero.position);
        });

    snx::PublisherStatic::addSubscriber(
        EventId::HERO_POSITION_CHANGED,
        [&]()
        {
            //* VisibilitySystem
            VisibilitySystem::update(
                scene.fogs,
                scene.game.world.currentMap->tiles.visibilityIds,
                scene.game.world.currentMap->tiles.ids,
                scene.game.world.currentMap->tiles.isOpaques,
                GameCameraModule::viewportInTiles(scene.gameCamera),
                scene.game.hero.visionRange,
                Convert::worldToTile(scene.game.hero.position));
        },
        true);

    snx::PublisherStatic::addSubscriber(
        EventId::MAP_CHANGE,
        [&]()
        {
            ChunkSystem::init(
                scene.renderData.textures,
                scene.chunks,
                scene.game.world.currentMap->tiles.ids,
                scene.game.world.currentMap->tiles.positions,
                scene.game.world.currentMap->tiles.renderIds);
        });

    snx::PublisherStatic::addSubscriber(
        EventId::COLOR_THEME_CHANGE,
        [&]()
        {
            RenderSystem::cycleThemes(scene.renderData.theme);
            RenderSystem::loadRenderData(scene.renderData);

            snx::PublisherStatic::publish(EventId::MAP_CHANGE);
        });

    snx::PublisherStatic::addSubscriber(
        EventId::CURSOR_TOGGLE,
        [&]()
        {
            CursorModule::toggle(scene.cursor);
        });

#if defined(DEBUG) && defined(DEBUG_TILEINFO)
    snx::PublisherStatic::addSubscriber(
        EventId::CURSOR_POSITION_CHANGED,
        [&]()
        {
            Vector2I cursorPos{Convert::worldToTile(scene.cursor.position)};
            Tiles& tiles{scene.game.world.currentMap->tiles};
            Objects& objects{scene.game.world.currentMap->objects};
            Enemies& enemies{scene.game.world.currentMap->enemies};

            if (!tiles.ids.contains(cursorPos))
            {
                return;
            }

            size_t tileId{tiles.ids.at(cursorPos)};

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
                + std::to_string(tiles.positions.at(tileId).x)
                + ", "
                + std::to_string(tiles.positions.at(tileId).y)
                + "\n");

            snx::debug::cliLog(
                "RenderId: "
                + std::to_string(static_cast<int>(tiles.renderIds.at(tileId)))
                + "\n");

            snx::debug::cliLog(
                "VisibilityId: "
                + std::to_string(static_cast<int>(tiles.visibilityIds.at(tileId)))
                + "\n");

            snx::debug::cliLog(
                "IsSolid: "
                + std::to_string(tiles.isSolids.contains(tileId))
                + "\n");

            snx::debug::cliLog(
                "IsOpaque: "
                + std::to_string(tiles.isOpaques.contains(tileId))
                + "\n");

            if (objects.ids.contains(cursorPos))
            {
                size_t objectId{objects.ids.at(cursorPos)};

                snx::debug::cliLog("OBJECT\n");

                snx::debug::cliLog(
                    "\nName: "
                    + objects.names.at(objectId)
                    + "\n");

                snx::debug::cliLog(
                    "\nActions: "
                    + objects.actions.at(objectId)
                    + "\n");

                snx::debug::cliLog(
                    "RenderId: "
                    + std::to_string(static_cast<int>(objects.renderIds.at(objectId)))
                    + "\n");

                snx::debug::cliLog(
                    "Event: "
                    + std::to_string(static_cast<int>(objects.events.at(objectId)))
                    + "\n");
            }

            if (enemies.ids.contains(cursorPos))
            {
                snx::debug::cliLog("ENEMY\n");

                snx::debug::cliLog(
                    "Id: "
                    + std::to_string(enemies.ids.at(cursorPos))
                    + "\n");
            }
        });
#endif
}

void SceneModule::processInput(SceneMain& scene)
{
    //* Color theme
    if (IsKeyPressed(KEY_F2))
    {
        snx::PublisherStatic::publish(EventId::COLOR_THEME_CHANGE);
    }

    //* Allow input if hero is ready (= full energy)
    if (scene.game.hero.energy.state == EnergyState::READY)
    {
        GameModule::processInput(
            scene.game.inputMappings,
            scene.cursor.isActive,
            scene.game.inputHandler,
            scene.game.inputAction);
    }
}

void SceneModule::updateState(SceneMain& scene)
{
    CursorModule::update(
        scene.cursor,
        scene.gameCamera.camera,
        Convert::worldToTile(scene.game.hero.position));

    GameModule::updateState(
        scene.game,
        scene.gameCamera,
        scene.cursor);

#if defined(DEBUG)
    snx::debug::gcam() = scene.gameCamera;
#endif
}

void SceneModule::renderOutput(SceneMain& scene)
{
    //* Draw map panel content
    BeginMode2D(scene.gameCamera.camera);
    BeginScissorMode(
        scene.gameCamera.viewport->left(),
        scene.gameCamera.viewport->top(),
        scene.gameCamera.viewport->width(),
        scene.gameCamera.viewport->height());

    //* World
    //* Draw map
    //* Draw chunks of tiles
    for (size_t idx{0}; idx < scene.chunks.values().size(); ++idx)
    {
        RenderSystem::renderChunk(
            scene.chunks.values().at(idx).renderTexture.texture,
            scene.chunks.values().at(idx).position);
    }

    //* Draw objects
    Objects const& objects{scene.game.world.currentMap->objects};

    for (size_t idx{0}; idx < objects.positions.size(); ++idx)
    {
        RenderSystem::renderTile(
            scene.renderData.textures,
            objects.renderIds.values().at(idx),
            objects.positions.values().at(idx));
    }

    //* Draw enemies
    Enemies const& enemies{scene.game.world.currentMap->enemies};
    Tiles const& tiles{scene.game.world.currentMap->tiles};

    for (size_t idx{0}; idx < enemies.positions.size(); ++idx)
    {
        Vector2 const& enemyPosition{enemies.positions.values().at(idx)};

        if (tiles.visibilityIds.values().at(idx) != VisibilityId::VISIBILE)
        {
            continue;
        }

        RenderSystem::renderTile(
            scene.renderData.textures,
            enemies.renderIds.values().at(idx),
            enemyPosition);
    }

    //* VisibilitySystem
    for (size_t idx{0}; idx < scene.fogs.values().size(); ++idx)
    {
        RenderSystem::renderFog(
            Convert::tileToWorld(scene.fogs.key(idx)),
            scene.fogs.values().at(idx));
    }

    //* Units
    //* Draw hero
    RenderSystem::renderTile(
        scene.renderData.textures,
        scene.game.hero.renderId,
        scene.game.hero.position);

    //* UI
    //* Draw cursor
    if (scene.cursor.isActive)
    {
        RenderSystem::renderTile(
            scene.renderData.textures,
            scene.cursor.renderId,
            scene.cursor.position);
    }

    EndScissorMode();
    EndMode2D();

    PanelSystem::drawLogPanelContent(scene.panels);

    PanelSystem::drawHeroInfoPanelContent(
        scene.panels,
        scene.game.hero);

    PanelSystem::drawTileInfoPanelContent(
        scene.panels,
        scene.game.world.currentMap->objects,
        Convert::worldToTile(scene.cursor.position));

    PanelSystem::drawGameInfoPanelContent(
        scene.panels,
        scene.game.world.currentMapLevel);

    PanelSystem::drawPanelBorders(scene.panels);
}

void SceneModule::update(SceneMain& scene)
{
    processInput(scene);
    updateState(scene);

    BeginDrawing();

    ClearBackground(Colors::bg);

    renderOutput(scene);

    //* Draw simple frame
    drawSceneBorder();

    if (DeveloperMode::isActive())
    {
        DrawFPS(0, 0);
    }

    EndDrawing();
}

void SceneModule::deinitialize(SceneMain& scene)
{
    RenderSystem::deinit(scene.renderData.textures);
}

void SceneModule::drawSceneBorder()
{
    DrawRectangleLinesEx(
        GetWindowRec(),
        SceneData::borderWidth,
        Colors::border);
}
