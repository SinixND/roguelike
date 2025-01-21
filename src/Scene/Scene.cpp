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
            PanelSystem::init( scene.panels );
        },
        true
    );

    snx::PublisherStatic::addSubscriber(
        EventId::PANELS_RESIZED,
        [&]()
        {
            GameCameraModule::init(
                scene.gameCamera,
                scene.panels.map,
                scene.game.hero.position
            );

            VisibilitySystem::update(
                scene.fogs,
                scene.game.world.currentMap->tiles.visibilityIds,
                scene.game.world.currentMap->tiles.isOpaques,
                GameCameraModule::viewportInTiles( scene.gameCamera ),
                scene.game.hero.visionRange,
                Convert::worldToTile( scene.game.hero.position )
            );
        }
    );

    snx::PublisherStatic::addSubscriber(
        EventId::CHANGE_COLOR_THEME,
        [&]()
        {
            RenderSystem::cycleThemes( scene.renderData.theme );
            RenderSystem::loadRenderData( scene.renderData );

            snx::PublisherStatic::publish( EventId::MAP_CHANGE );
        }
    );

    //* Game events
    snx::PublisherStatic::addSubscriber(
        EventId::HERO_MOVED,
        [&]()
        {
            GameCameraModule::setTarget(
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
            VisibilitySystem::update(
                scene.fogs,
                scene.game.world.currentMap->tiles.visibilityIds,
                scene.game.world.currentMap->tiles.isOpaques,
                GameCameraModule::viewportInTiles( scene.gameCamera ),
                scene.game.hero.visionRange,
                Convert::worldToTile( scene.game.hero.position )
            );
        },
        true
    );

    snx::PublisherStatic::addSubscriber(
        EventId::MAP_CHANGE,
        [&]()
        {
            ChunkSystem::init(
                scene.renderData.textures,
                scene.chunks,
                scene.game.world.currentMap->tiles.positions,
                scene.game.world.currentMap->tiles.renderIds
            );
        }
    );

#if defined( DEBUG ) && defined( DEBUG_TILEINFO )
    snx::PublisherStatic::addSubscriber(
        EventId::CURSOR_POSITION_CHANGED,
        [&]()
        {
            Vector2I cursorPos{ Convert::worldToTile( cursor.position ) };

            if ( !scene.game.world.currentMap->tiles.positions.contains( cursorPos ) )
            {
                return;
            }

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
                + std::to_string( scene.game.world.currentMap->tiles.positions.at( cursorPos ).x )
                + ", "
                + std::to_string( scene.game.world.currentMap->tiles.positions.at( cursorPos ).y )
                + "\n"
            );

            snx::debug::cliLog(
                "RenderId: "
                + std::to_string( static_cast<int>( scene.game.world.currentMap->tiles.renderIds.at( cursorPos ) ) )
                + "\n"
            );

            snx::debug::cliLog(
                "VisibilityId: "
                + std::to_string( static_cast<int>( scene.game.world.currentMap->tiles.visibilityIds.at( cursorPos ) ) )
                + "\n"
            );

            snx::debug::cliLog(
                "IsSolid: "
                + std::to_string( scene.game.world.currentMap->tiles.isSolids.contains( cursorPos ) )
                + "\n"
            );

            snx::debug::cliLog(
                "IsOpaque: "
                + std::to_string( scene.game.world.currentMap->tiles.isOpaques.contains( cursorPos ) )
                + "\n"
            );

            if ( scene.game.world.currentMap->objects.positions.contains( cursorPos ) )
            {
                snx::debug::cliLog( "OBJECT\n" );

                snx::debug::cliLog(
                    "\nName: "
                    + scene.game.world.currentMap->objects.names.at( cursorPos )
                    + "\n"
                );

                snx::debug::cliLog(
                    "\nActions: "
                    + scene.game.world.currentMap->objects.actions.at( cursorPos )
                    + "\n"
                );

                snx::debug::cliLog(
                    "RenderId: "
                    + std::to_string( static_cast<int>( scene.game.world.currentMap->objects.renderIds.at( cursorPos ) ) )
                    + "\n"
                );

                snx::debug::cliLog(
                    "Event: "
                    + std::to_string( static_cast<int>( scene.game.world.currentMap->objects.events.at( cursorPos ) ) )
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
    Scene& scene,
    Cursor const& cursor
)
{
    //* Draw map panel content
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
    auto const& objects{ scene.game.world.currentMap->objects };
    auto const& objectRenderIds{ objects.renderIds.values() };
    auto const& objectPositions{ objects.positions.values() };

    for ( size_t idx{ 0 }; idx < objectRenderIds.size(); ++idx )
    {
        RenderSystem::renderTile(
            scene.renderData.textures,
            objectRenderIds.at( idx ),
            objectPositions.at( idx )
        );
    }

    //* Draw enemies
    auto const& enemies{ scene.game.world.currentMap->enemies };
    auto const& enemyRenderIds{ enemies.renderIds.values() };
    auto const& enemyPositions{ enemies.positions.values() };

    for ( size_t idx{ 0 }; idx < enemyRenderIds.size(); ++idx )
    {
        if ( scene.game.world.currentMap->tiles.visibilityIds.at( Convert::worldToTile( enemyPositions.at( idx ) ) ) != VisibilityId::VISIBILE )
        {
            continue;
        }

        RenderSystem::renderTile(
            scene.renderData.textures,
            enemyRenderIds.at( idx ),
            enemyPositions.at( idx )
        );
    }

    //* VisibilitySystem
    for ( size_t idx{ 0 }; idx < scene.fogs.size(); ++idx )
    {
        RenderSystem::renderFog(
            Convert::tileToWorld( scene.fogs.key( idx ) ),
            scene.fogs.values().at( idx )
        );
    }

    //* Units
    //* Draw hero
    RenderSystem::renderTile(
        scene.renderData.textures,
        scene.game.hero.renderId,
        scene.game.hero.position
    );

    //* UI
    //* Draw cursor
    if ( cursor.isActive )
    {
        RenderSystem::renderTile(
            scene.renderData.textures,
            cursor.renderId,
            cursor.position
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

void drawSceneBorder()
{
    DrawRectangleLinesEx(
        GetWindowRec(),
        SceneData::borderWeight,
        Colors::border
    );
}

namespace SceneModule
{
    void init(
        Scene& scene,
        Cursor const& cursor
    )
    {
        GameModule::init( scene.game );

        PanelSystem::init( scene.panels );

        GameCameraModule::init(
            scene.gameCamera,
            scene.panels.map,
            scene.game.hero.position
        );

#if defined( DEBUG )
        snx::debug::gcam() = scene.gameCamera;
#endif

        RenderSystem::loadRenderData( scene.renderData );

        ChunkSystem::init(
            scene.renderData.textures,
            scene.chunks,
            scene.game.world.currentMap->tiles.positions,
            scene.game.world.currentMap->tiles.renderIds
        );

        //* Setup events
        setupSceneEvents(
            scene,
            cursor
        );
    }

    void update(
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

        CursorModule::update(
            cursor,
            scene.gameCamera.camera,
            Convert::worldToTile( scene.game.hero.position )
        );

        GameModule::update(
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
        drawSceneBorder();

        if ( DeveloperMode::isActive() )
        {
            DrawFPS( 0, 0 );
        }

        EndDrawing();
    }

    void deinitialize( Scene& scene )
    {
        RenderSystem::deinit( scene.renderData.textures );
    }
}
