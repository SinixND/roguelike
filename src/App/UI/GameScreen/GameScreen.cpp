#include "GameScreen.h"

#include "ChunkSystem.h"
#include "ColorData.h"
#include "Cursor.h"
#include "Debugger.h"
#include "DeveloperMode.h"
#include "Enemies.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "Game.h"
#include "GameCamera.h"
#include "GamePanels.h"
#include "GameState.h"
#include "InputId.h"
#include "Objects.h"
#include "RenderSystem.h"
#include "UISystem.h"
#include "VisibilitySystem.h"
#include "World.h"
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

void setupScreenEvents(
    ScreenGame& screen,
    Hero const& hero,
    World const& world
)
{
    snx::EventDispatcher::addListener(
        EventId::CHANGE_COLOR_THEME,
        [&]()
        {
            screen.renderData.theme = RenderSystem::cycleThemes( screen.renderData.theme );

            screen.renderData = RenderSystem::loadRenderData( screen.renderData );

            snx::EventDispatcher::notify( EventId::MAP_CHANGE );
        }
    );

    //* Game events
    snx::EventDispatcher::addListener(
        EventId::HERO_MOVED,
        [&]()
        {
            screen.gameCamera.camera = GameCameraModule::setTarget(
                screen.gameCamera.camera,
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
                GameCameraModule::viewportInTiles( screen.gameCamera ),
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
            screen.chunks = ChunkSystem::reRenderChunks(
                screen.chunks,
                screen.renderData.textures,
                world.currentMap->tiles
            );
        }
    );
}

void renderOutput(
    ScreenGame const& screen,
    Hero const& hero,
    Map const& currentMap,
    Cursor const& cursor,
    int currentMapLevel
)
{
    //* Draw viewport content
    BeginMode2D( screen.gameCamera.camera );
    BeginScissorMode(
        screen.panelComponents.map.inner().left(),
        screen.panelComponents.map.inner().top(),
        screen.panelComponents.map.inner().width(),
        screen.panelComponents.map.inner().height()
    );

    //* World
    //* Draw map
    //* Draw tiles
    for ( Chunk const& chunk : screen.chunks )
    {
        RenderSystem::renderChunk( chunk );
    }

    //* Draw objects
    Objects const& objects{ currentMap.objects };

    for ( size_t idx{ 0 }; idx < objects.renderIds.values().size(); ++idx )
    {
        RenderSystem::renderTexture(
            screen.renderData.textures,
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
            screen.renderData.textures,
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
        screen.renderData.textures,
        hero.position,
        hero.renderId
    );

    //* UI
    //* Draw cursor
    if ( cursor.isActive )
    {
        RenderSystem::renderTexture(
            screen.renderData.textures,
            cursor.position,
            cursor.renderId
        );
    }

    EndScissorMode();
    EndMode2D();

    Color bgColor{ 0, 0, 0, 127 };
    float borderWidth{ 1.0f };

    RenderSystem::renderStatusPanel(
        screen.panelComponents.status,
        hero,
        currentMapLevel,
        bgColor,
        0
    );

    RenderSystem::renderInfoPanel(
        screen.panelComponents.info,
        currentMap.objects,
        Convert::worldToTile( cursor.position ),
        bgColor,
        borderWidth
    );

    RenderSystem::renderLogPanel(
        screen.panelComponents.log,
        bgColor,
        0
    );
}

namespace ScreenGameModule
{
    ScreenGame const& init(
        ScreenGame& gameScreen,
        Hero const& hero,
        World const& world
    )
    {
        gameScreen.panelComponents = GamePanelsModule::init( gameScreen.panelComponents );
        gameScreen.overlays = OverlaysModule::init( gameScreen.overlays );

        gameScreen.gameCamera = GameCameraModule::init(
            gameScreen.gameCamera,
            gameScreen.panelComponents.map.box(),
            hero.position
        );

#if defined( DEBUG )
        snx::Debugger::gcam() = gameScreen.gameCamera;
#endif

        gameScreen.renderData = RenderSystem::loadRenderData( gameScreen.renderData );

        gameScreen.chunks = ChunkSystem::reRenderChunks(
            gameScreen.chunks,
            gameScreen.renderData.textures,
            world.currentMap->tiles
        );

        //* Setup events
        setupScreenEvents(
            gameScreen,
            hero,
            world
        );

        return gameScreen;
    }

    ScreenGame const& update(
        ScreenGame& gameScreen,
        Hero& heroIO,
        World const& world,
        Cursor const& cursor,
        GameState gameState,
        InputId currentInputId
    )
    {
        if ( currentInputId == InputId::CYCLE_THEME )
        {
            snx::EventDispatcher::notify( EventId::CHANGE_COLOR_THEME );
        }

#if defined( DEBUG )
        snx::Debugger::gcam() = gameScreen.gameCamera;
#endif
        BeginDrawing();
        ClearBackground( ColorData::BG );

        renderOutput(
            gameScreen,
            heroIO,
            *world.currentMap,
            cursor,
            world.currentMapLevel
        );

        switch ( gameState )
        {
            default:
            case GameState::DEFAULT:
                break;

            case GameState::LEVEL_UP:
            {
                gameScreen.overlays.levelUp = OverlayLevelUpModule::update(
                    gameScreen.overlays.levelUp,
                    heroIO,
                    currentInputId
                );
                break;
            }
        }

        //* Draw simple frame
        UISystem::drawWindowBorder();

        if ( DeveloperMode::isActive() )
        {
            DrawFPS( 0, 0 );
        }

        EndDrawing();

        return gameScreen;
    }

    void deinitialize( ScreenGame& gameScreen )
    {
        RenderSystem::deinit( gameScreen.renderData.textures );
    }
}
