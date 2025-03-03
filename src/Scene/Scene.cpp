#include "Scene.h"

#include "ChunkSystem.h"
#include "Colors.h"
#include "Cursor.h"
#include "Debugger.h"
#include "DeveloperMode.h"
#include "Enemies.h"
#include "EventDispatcher.h"
#include "Events.h"
#include "Game.h"
#include "GameCamera.h"
#include "InputId.h"
#include "Objects.h"
#include "PanelSystem.h"
#include "RenderSystem.h"
#include "SceneData.h"
#include "VisibilityId.h"
#include "VisibilitySystem.h"
#include "World.h"
#include "raylibEx.h"
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

void setupSceneEvents(
    Scene& scene,
    Hero const& hero,
    World const& world
)
{
    snx::EventDispatcher::addListener(
        EventId::CHANGE_COLOR_THEME,
        [&]()
        {
            RenderSystem::cycleThemes( scene.renderData.theme );

            scene.renderData = RenderSystem::loadRenderData( scene.renderData );

            snx::EventDispatcher::notify( EventId::MAP_CHANGE );
        }
    );

    //* Game events
    snx::EventDispatcher::addListener(
        EventId::HERO_MOVED,
        [&]()
        {
            scene.gameCamera.camera = GameCameraModule::setTarget(
                scene.gameCamera.camera,
                hero.position
            );
        }
    );

    snx::EventDispatcher::addListener(
        EventId::HERO_POSITION_CHANGED,
        [&]()
        {
            //* VisibilitySystem
            world.currentMap->tiles = VisibilitySystem::calculateVisibilities(
                world.currentMap->tiles,
                world.currentMap->fogs,
                GameCameraModule::viewportInTiles( scene.gameCamera ),
                Convert::worldToTile( hero.position ),
                hero.visionRange
            );
        },
        true
    );

    snx::EventDispatcher::addListener(
        EventId::MAP_CHANGE,
        [&]()
        {
            scene.chunks = ChunkSystem::reRenderChunks(
                scene.chunks,
                scene.renderData.textures,
                world.currentMap->tiles
            );
        }
    );
}

void renderOutput(
    Scene const& scene,
    Hero const& hero,
    Map const& currentMap,
    Cursor const& cursor,
    int currentMapLevel
)
{
    //* Draw viewport content
    BeginMode2D( scene.gameCamera.camera );
    BeginScissorMode(
        scene.panels.map.inner().left(),
        scene.panels.map.inner().top(),
        scene.panels.map.inner().width(),
        scene.panels.map.inner().height()
    );

    //* World
    //* Draw map
    //* Draw tiles
    for ( Chunk const& chunk : scene.chunks )
    {
        RenderSystem::renderChunk( chunk );
    }

    //* Draw objects
    Objects const& objects{ currentMap.objects };

    for ( size_t idx{ 0 }; idx < objects.renderIds.values().size(); ++idx )
    {
        RenderSystem::renderTexture(
            scene.renderData.textures,
            objects.positions.values()[idx],
            objects.renderIds.values()[idx]
        );
    }

    //* Draw enemies
    Tiles const& tiles{ currentMap.tiles };
    Enemies const& enemies{ currentMap.enemies };

    for ( size_t idx{ 0 }; idx < enemies.renderIds.values().size(); ++idx )
    {
        if (
            tiles.visibilityIds[tiles.ids.index(
                Convert::worldToTile(
                    enemies.positions.values()[idx]
                )
            )]
            != VisibilityId::VISIBILE
        )
        {
            continue;
        }

        RenderSystem::renderTexture(
            scene.renderData.textures,
            enemies.positions.values()[idx],
            enemies.renderIds.values()[idx]
        );
    }

    //* Fog of war
    auto const& fogs{ currentMap.fogs };

    for ( size_t idx{ 0 }; idx < fogs.size(); ++idx )
    {
        RenderSystem::renderFog(
            Convert::tileToWorld( fogs.key( idx ) ),
            fogs[idx]
        );
    }

    //* Units
    //* Draw hero
    RenderSystem::renderTexture(
        scene.renderData.textures,
        hero.position,
        hero.renderId
    );

    //* UI
    //* Draw cursor
    if ( cursor.isActive )
    {
        RenderSystem::renderTexture(
            scene.renderData.textures,
            cursor.position,
            cursor.renderId
        );
    }

    EndScissorMode();
    EndMode2D();

    Color bgColor{ 0, 0, 0, 127 };
    float borderWidth{ 1.0f };

    RenderSystem::renderStatusPanel(
        scene.panels.status,
        hero,
        currentMapLevel,
        bgColor,
        0
    );

    RenderSystem::renderInfoPanel(
        scene.panels.info,
        currentMap.objects,
        Convert::worldToTile( cursor.position ),
        bgColor,
        borderWidth
    );

    RenderSystem::renderLogPanel(
        scene.panels.log,
        bgColor,
        0
    );
}

void drawWindowBorder()
{
    DrawRectangleLinesEx(
        GetWindowRec(),
        SceneData::borderWeight,
        Colors::border
    );
}

namespace SceneModule
{
    [[nodiscard]]
    Scene const& init(
        Scene& scene,
        Hero const& hero,
        World const& world
    )
    {
        scene.panels = PanelSystem::init( scene.panels );

        scene.gameCamera = GameCameraModule::init(
            scene.gameCamera,
            scene.panels.map.box(),
            hero.position
        );

#if defined( DEBUG )
        snx::debug::gcam() = scene.gameCamera;
#endif

        scene.renderData = RenderSystem::loadRenderData( scene.renderData );

        scene.chunks = ChunkSystem::reRenderChunks(
            scene.chunks,
            scene.renderData.textures,
            world.currentMap->tiles
        );

        //* Setup events
        setupSceneEvents(
            scene,
            hero,
            world
        );

        return scene;
    }

    [[nodiscard]]
    Scene const& update(
        Scene& scene,
        Hero const& hero,
        World const& world,
        Cursor& cursor,
        InputId currentInputId
    )
    {
        if ( currentInputId == InputId::CYCLE_THEME )
        {
            snx::EventDispatcher::notify( EventId::CHANGE_COLOR_THEME );
        }

#if defined( DEBUG )
        snx::debug::gcam() = scene.gameCamera;
#endif
        BeginDrawing();
        ClearBackground( Colors::bg );

        renderOutput(
            scene,
            hero,
            *world.currentMap,
            cursor,
            world.currentMapLevel
        );

        //* Draw simple frame
        drawWindowBorder();

        if ( DeveloperMode::isActive() )
        {
            DrawFPS( 0, 0 );
        }

        EndDrawing();

        return scene;
    }

    void deinitialize( Scene& scene )
    {
        RenderSystem::deinit( scene.renderData.textures );
    }
}
