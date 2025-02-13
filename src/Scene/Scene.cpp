#include "Scene.h"

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
#include "InputId.h"
#include "Objects.h"
#include "PanelSystem.h"
#include "PublisherStatic.h"
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
    Cursor const& cursor
)
{
    snx::PublisherStatic::addSubscriber(
        EventId::WINDOW_RESIZED,
        [&]()
        {
            scene.panels = PanelSystem::init( scene.panels );

            scene.gameCamera = GameCameraModule::init(
                scene.gameCamera,
                scene.panels.map,
                scene.game.hero.position
            );

            scene.game.world.currentMap->tiles = VisibilitySystem::calculateVisibilities(
                scene.game.world.currentMap->tiles,
                scene.game.world.currentMap->fogs,
                GameCameraModule::viewportInTiles( scene.gameCamera ),
                Convert::worldToTile( scene.game.hero.position ),
                scene.game.hero.visionRange
            );
        },
        true
    );

    snx::PublisherStatic::addSubscriber(
        EventId::CHANGE_COLOR_THEME,
        [&]()
        {
            RenderSystem::cycleThemes( scene.renderData.theme );
            scene.renderData = RenderSystem::loadRenderData( scene.renderData );

            snx::PublisherStatic::publish( EventId::MAP_CHANGE );
        }
    );

    //* Game events
    snx::PublisherStatic::addSubscriber(
        EventId::HERO_MOVED,
        [&]()
        {
            scene.gameCamera = GameCameraModule::setTarget(
                scene.gameCamera,
                scene.game.hero.position
            );
        }
    );

    snx::PublisherStatic::addSubscriber(
        EventId::HERO_POSITION_CHANGED,
        [&]()
        {
            //* VisibilitySystem
            scene.game.world.currentMap->tiles = VisibilitySystem::calculateVisibilities(
                scene.game.world.currentMap->tiles,
                scene.game.world.currentMap->fogs,
                GameCameraModule::viewportInTiles( scene.gameCamera ),
                Convert::worldToTile( scene.game.hero.position ),
                scene.game.hero.visionRange
            );
        },
        true
    );

    snx::PublisherStatic::addSubscriber(
        EventId::MAP_CHANGE,
        [&]()
        {
            scene.chunks = ChunkSystem::renderToChunks(
                scene.chunks,
                scene.renderData.textures,
                scene.game.world.currentMap->tiles
            );
        }
    );

#if defined( DEBUG ) && defined( DEBUG_TILEINFO )
    snx::PublisherStatic::addSubscriber(
        EventId::CURSOR_POSITION_CHANGED,
        [&]()
        {
            Vector2I cursorPos{ Convert::worldToTile( cursor.position ) };

            Tiles& tiles{ scene.game.world.currentMap->tiles };

            if ( !tiles.ids.contains( cursorPos ) )
            {
                return;
            }

            size_t tileId{ tiles.ids.at( cursorPos ) };

            snx::debug::cliPrint( "\n" );
            snx::debug::cliLog( "TILE:\n" );

            snx::debug::cliLog(
                "TilePosition: "
                + std::to_string( cursorPos.x )
                + ", "
                + std::to_string( cursorPos.y )
                + "\n"
            );

            snx::debug::cliLog(
                "WorldPixel: "
                + std::to_string( tiles.positions.at( tileId ).x )
                + ", "
                + std::to_string( tiles.positions.at( tileId ).y )
                + "\n"
            );

            snx::debug::cliLog(
                "RenderId: "
                + std::to_string( static_cast<int>( tiles.renderIds.at( tileId ) ) )
                + "\n"
            );

            snx::debug::cliLog(
                "VisibilityId: "
                + std::to_string( static_cast<int>( tiles.visibilityIds.at( tileId ) ) )
                + "\n"
            );

            snx::debug::cliLog(
                "IsSolid: "
                + std::to_string( tiles.isSolids.contains( tileId ) )
                + "\n"
            );

            snx::debug::cliLog(
                "IsOpaque: "
                + std::to_string( tiles.isOpaques.contains( tileId ) )
                + "\n"
            );

            Objects& objects{ scene.game.world.currentMap->objects };

            if ( objects.ids.contains( Convert::worldToTile( cursor.position ) ) )
            {
                size_t objectId{ objects.ids.at( Convert::worldToTile( cursor.position ) ) };

                snx::debug::cliLog( "OBJECT\n" );

                snx::debug::cliLog(
                    "\nName: "
                    + scene.game.world.currentMap->objects.names.at( objectId )
                    + "\n"
                );

                snx::debug::cliLog(
                    "\nActions: "
                    + scene.game.world.currentMap->objects.actions.at( objectId )
                    + "\n"
                );

                snx::debug::cliLog(
                    "RenderId: "
                    + std::to_string( static_cast<int>( scene.game.world.currentMap->objects.renderIds.at( objectId ) ) )
                    + "\n"
                );

                snx::debug::cliLog(
                    "Event: "
                    + std::to_string( static_cast<int>( scene.game.world.currentMap->objects.events.at( objectId ) ) )
                    + "\n"
                );
            }

            if ( scene.game.world.currentMap->enemies.ids.contains( cursorPos ) )
            {
                snx::debug::cliLog( "ENEMY\n" );

                snx::debug::cliLog(
                    "Id: "
                    + std::to_string( scene.game.world.currentMap->enemies.ids.at( cursorPos ) )
                    + "\n"
                );
            }
        }
    );
#endif
}

void renderOutput(
    Scene const& scene,
    Cursor const& cursor
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

    //* World
    //* Draw map
    //* Draw tiles
    for ( Chunk const& chunk : scene.chunks )
    {
        RenderSystem::renderChunk( chunk );
    }

    //* Draw objects
    Objects const& objects{ scene.game.world.currentMap->objects };

    for ( size_t idx{ 0 }; idx < objects.renderIds.values().size(); ++idx )
    {
        RenderSystem::renderTexture(
            scene.renderData.textures,
            objects.positions.values()[idx],
            objects.renderIds.values()[idx]
        );
    }

    //* Draw enemies
    Tiles const& tiles{ scene.game.world.currentMap->tiles };
    Enemies const& enemies{ scene.game.world.currentMap->enemies };

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
    auto const& fogs{ scene.game.world.currentMap->fogs };

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
        scene.game.hero.position,
        scene.game.hero.renderId
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
        scene.game.hero
    );

    PanelSystem::drawTileInfoPanelContent(
        scene.panels,
        scene.game.world.currentMap->objects,
        Convert::worldToTile( cursor.position )
    );

    PanelSystem::drawGameInfoPanelContent(
        scene.panels,
        scene.game.world.currentMapLevel
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
        Cursor const& cursor
    )
    {
        scene.game = GameModule::init( scene.game );

        scene.panels = PanelSystem::init( scene.panels );

        scene.gameCamera = GameCameraModule::init(
            scene.gameCamera,
            scene.panels.map,
            scene.game.hero.position
        );

#if defined( DEBUG )
        snx::debug::gcam() = scene.gameCamera;
#endif

        scene.renderData = RenderSystem::loadRenderData( scene.renderData );

        scene.chunks = ChunkSystem::renderToChunks(
            scene.chunks,
            scene.renderData.textures,
            scene.game.world.currentMap->tiles
        );

        //* Setup events
        setupSceneEvents(
            scene,
            cursor
        );

        return scene;
    }

    [[nodiscard]]
    Scene const& update(
        Scene& scene,
        Cursor& cursor,
        InputId currentInputId,
        float dt
    )
    {
        if ( currentInputId == InputId::CYCLE_THEME )
        {
            snx::PublisherStatic::publish( EventId::CHANGE_COLOR_THEME );
        }

        cursor = CursorModule::update(
            cursor,
            scene.gameCamera.camera,
            Convert::worldToTile( scene.game.hero.position )
        );

        scene.game = GameModule::update(
            scene.game,
            scene.gameCamera,
            cursor,
            currentInputId,
            dt
        );

#if defined( DEBUG )
        snx::debug::gcam() = scene.gameCamera;
#endif
        BeginDrawing();
        ClearBackground( Colors::bg );

        renderOutput(
            scene,
            cursor
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
