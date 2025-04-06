#include "GameScreen.h"

#include "ChunkSystem.h"
#include "ColorData.h"
#include "Cursor.h"
#include "DeveloperMode.h"
#include "Enemies.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "Game.h"
#include "GameCamera.h"
#include "GamePanels.h"
#include "InputId.h"
#include "Objects.h"
#include "RenderSystem.h"
#include "VisibilitySystem.h"
#include "WindowSystem.h"
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

void GameScreen::setupScreenEvents( World const& world )
{
    snx::EventDispatcher::addListener(
        EventId::CHANGE_COLOR_THEME,
        [&]()
        {
            renderData.theme = RenderSystem::cycleThemes( renderData.theme );

            renderData = RenderSystem::loadRenderData( renderData );

            snx::EventDispatcher::notify( EventId::MAP_CHANGE );
        }
    );

    //* Game events
    snx::EventDispatcher::addListener(
        EventId::MAP_CHANGE,
        [&]()
        {
            chunks = ChunkSystem::reRenderChunks(
                chunks,
                renderData.textures,
                world.currentMap->tiles
            );
        }
    );
}

void GameScreen::renderOutput(
    Hero const& hero,
    Map const& currentMap,
    int currentMapLevel,
    Cursor const& cursor,
    GameCamera const& gameCamera
)
{
    //* Draw viewport content
    BeginMode2D( gameCamera.camera );
    BeginScissorMode(
        panels.map.inner().left(),
        panels.map.inner().top(),
        panels.map.inner().width(),
        panels.map.inner().height()
    );

    //* World
    //* Draw map
    //* Draw tiles
    for ( Chunk const& chunk : chunks )
    {
        RenderSystem::renderChunk( chunk );
    }

    //* Draw objects
    Objects const& objects{ currentMap.objects };

    for ( size_t idx{ 0 }; idx < objects.renderIds.values().size(); ++idx )
    {
        RenderSystem::renderTexture(
            renderData.textures,
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
#if !defined( DEBUG )
            continue;
#endif
        }

        RenderSystem::renderTexture(
            renderData.textures,
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
        renderData.textures,
        hero.position,
        hero.renderId
    );

    //* UI
    //* Draw cursor
    if ( cursor.isActive )
    {
        RenderSystem::renderTexture(
            renderData.textures,
            cursor.position,
            cursor.renderId
        );
    }

    EndScissorMode();
    EndMode2D();

    Color bgColor{ 0, 0, 0, 127 };
    float borderWidth{ 1.0f };

    RenderSystem::renderStatusPanel(
        panels.status,
        hero,
        currentMapLevel,
        bgColor,
        0
    );

    RenderSystem::renderInfoPanel(
        panels.info,
        currentMap.objects,
        Convert::worldToTile( cursor.position ),
        bgColor,
        borderWidth
    );

    RenderSystem::renderLogPanel(
        panels.log,
        bgColor,
        0
    );
}

void GameScreen::init(
    World const& world,
    GameCamera const& gameCamera
)
{
    panels = GamePanelsModule::init( panels );
    levelUpOverlay.init();

#if defined( DEBUG )
    snx::Debugger::gcam() = gameCamera;
#endif

    renderData = RenderSystem::loadRenderData( renderData );

    chunks = ChunkSystem::reRenderChunks(
        chunks,
        renderData.textures,
        world.currentMap->tiles
    );

    //* Setup events
    setupScreenEvents( world );
}

void GameScreen::update(
    Hero const& hero,
    Map const& currentMap,
    int currentMapLevel,
    Cursor const& cursor,
    GameCamera const& gameCamera,
    InputId currentInputId
)
{
    if ( currentInputId == InputId::CYCLE_THEME )
    {
        snx::EventDispatcher::notify( EventId::CHANGE_COLOR_THEME );
    }

    BeginDrawing();
    ClearBackground( ColorData::BG );

    renderOutput(
        hero,
        currentMap,
        currentMapLevel,
        cursor,
        gameCamera
    );

    //* Draw simple frame
    WindowSystem::drawWindowBorder();

    if ( DeveloperMode::isActive() )
    {
        DrawFPS( 0, 0 );
    }

    EndDrawing();
}

void GameScreen::deinit()
{
    RenderSystem::deinit( renderData.textures );
}
