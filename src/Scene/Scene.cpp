#include "Scene.h"

#define DEBUG_TILEINFO
//* #define DEBUG_FOG

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
#include "Levels.h"
#include "Objects.h"
#include "PanelSystem.h"
#include "RenderSystem.h"
#include "SceneData.h"
#include "VisibilityId.h"
#include "VisibilitySystem.h"
#include "raylibEx.h"
#include <cstddef>
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

#if defined( DEBUG ) && defined( DEBUG_TILEINFO )
#include "Objects.h"
#include <string>
#endif

void setupSceneEvents(
    Scene& scene,
    Hero const& hero,
    Map& currentMap,
    Cursor const& cursor
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
            scene.gameCamera = GameCameraModule::setTarget(
                scene.gameCamera,
                hero.position
            );
        }
    );

    snx::EventDispatcher::addListener(
        EventId::HERO_POSITION_CHANGED,
        [&]()
        {
            //* VisibilitySystem
            currentMap.tiles = VisibilitySystem::calculateVisibilities(
                currentMap.tiles,
                currentMap.fogs,
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
                currentMap.tiles
            );
        }
    );

#if defined( DEBUG ) && defined( DEBUG_TILEINFO )
    snx::EventDispatcher::addListener(
        EventId::CURSOR_POSITION_CHANGED,
        [&]()
        {
            Vector2I cursorPos{ Convert::worldToTile( cursor.position ) };

            Tiles& tiles{ currentMap.tiles };

            if ( !tiles.ids.contains( cursorPos ) )
            {
                return;
            }

            size_t tileId{ tiles.ids.at( cursorPos ) };

            snx::debug::cliPrint( "\n" );
            snx::debug::cliLog( "TILE:\n" );

            snx::debug::cliPrint(
                "TilePosition: "
                + std::to_string( cursorPos.x )
                + ", "
                + std::to_string( cursorPos.y )
                + "\n"
            );

            snx::debug::cliPrint(
                "WorldPixel: "
                + std::to_string( tiles.positions.at( tileId ).x )
                + ", "
                + std::to_string( tiles.positions.at( tileId ).y )
                + "\n"
            );

            snx::debug::cliPrint(
                "RenderId: "
                + std::to_string( static_cast<int>( tiles.renderIds.at( tileId ) ) )
                + "\n"
            );

            snx::debug::cliPrint(
                "VisibilityId: "
                + std::to_string( static_cast<int>( tiles.visibilityIds.at( tileId ) ) )
                + "\n"
            );

            snx::debug::cliPrint(
                "IsSolid: "
                + std::to_string( tiles.isSolids.contains( tileId ) )
                + "\n"
            );

            snx::debug::cliPrint(
                "IsOpaque: "
                + std::to_string( tiles.isOpaques.contains( tileId ) )
                + "\n"
            );

            Objects& objects{ currentMap.objects };

            if ( objects.ids.contains( Convert::worldToTile( cursor.position ) ) )
            {
                size_t objectId{ objects.ids.at( Convert::worldToTile( cursor.position ) ) };

                snx::debug::cliLog( "OBJECT\n" );

                snx::debug::cliPrint(
                    "Name: "
                    + currentMap.objects.names.at( objectId )
                    + "\n"
                );

                snx::debug::cliPrint(
                    "Actions: "
                    + currentMap.objects.actions.at( objectId )
                    + "\n"
                );

                snx::debug::cliPrint(
                    "RenderId: "
                    + std::to_string( static_cast<int>( currentMap.objects.renderIds.at( objectId ) ) )
                    + "\n"
                );

                snx::debug::cliPrint(
                    "Event: "
                    + std::to_string( static_cast<int>( currentMap.objects.events.at( objectId ) ) )
                    + "\n"
                );
            }

            if ( currentMap.enemies.ids.contains( cursorPos ) )
            {
                snx::debug::cliLog( "ENEMY\n" );

                snx::debug::cliPrint(
                    "Id: "
                    + std::to_string( currentMap.enemies.ids.at( cursorPos ) )
                    + "\n"
                );
            }
        }
    );
#endif
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
        scene.gameCamera.viewport->left(),
        scene.gameCamera.viewport->top(),
        scene.gameCamera.viewport->width(),
        scene.gameCamera.viewport->height()
    );

    //* Levels
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
            tiles.visibilityIds.at(
                tiles.ids.at( Convert::worldToTile(
                    enemies.positions.values()[idx]
                ) )
            )
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

    PanelSystem::drawLogPanelContent( scene.panels );

    PanelSystem::drawHeroInfoPanelContent(
        scene.panels,
        hero
    );

    PanelSystem::drawTileInfoPanelContent(
        scene.panels,
        currentMap.objects,
        Convert::worldToTile( cursor.position )
    );

    PanelSystem::drawGameInfoPanelContent(
        scene.panels,
        currentMapLevel
    );

    PanelSystem::drawPanelBorders( scene.panels );
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
        Map& currentMap,
        Cursor const& cursor
    )
    {
        scene.panels = PanelSystem::init( scene.panels );

        scene.gameCamera = GameCameraModule::init(
            scene.gameCamera,
            scene.panels.map,
            hero.position
        );

#if defined( DEBUG )
        snx::debug::gcam() = scene.gameCamera;
#endif

        scene.renderData = RenderSystem::loadRenderData( scene.renderData );

        scene.chunks = ChunkSystem::reRenderChunks(
            scene.chunks,
            scene.renderData.textures,
            currentMap.tiles
        );

        //* Setup events
        setupSceneEvents(
            scene,
            hero,
            currentMap,
            cursor
        );

        return scene;
    }

    [[nodiscard]]
    Scene const& update(
        Scene& scene,
        Hero const& hero,
        Levels const& levels,
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
            *levels.currentMap,
            cursor,
            levels.currentMapLevel
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
