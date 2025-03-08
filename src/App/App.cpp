#include "App.h"

// #define DEBUG_TILEINFO

#include "AppConfig.h"
#include "DeveloperMode.h"
#include "EventDispatcher.h"
#include "EventId.h"
#include "Game.h"
#include "GameFont.h"
#include "InputHandler.h"
#include "InputId.h"
#include "PanelSystem.h"
#include "VisibilitySystem.h"
#include <raygui.h>
#include <raylib.h>

#if defined( EMSCRIPTEN )
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

#if defined( DEBUG ) && defined( DEBUG_TILEINFO )
#include "Objects.h"
#include <string>
#endif

void setupRaylib( AppConfig const& config )
{
    //* Raylib flags
    SetConfigFlags( FLAG_WINDOW_RESIZABLE );
    if ( config.vSync )
    {
        SetConfigFlags( FLAG_VSYNC_HINT );
    }

    //* Initialize window
    InitWindow( config.windowWidth, config.windowHeight, "Roguelike" );

    //* Raylib Settings
    SetWindowIcon( config.favicon );
    SetWindowMinSize( 640, 480 );
#if defined( EMSCRIPTEN )
    MaximizeWindow();
#endif
    SetExitKey( KEY_F4 );
    SetTargetFPS( config.fpsTarget );
    SetTextureFilter( GetFontDefault().texture, TEXTURE_FILTER_POINT );

    //* Fonts
    GameFont::load();
    GuiSetStyle( DEFAULT, TEXT_SIZE, GameFont::fontSize );
}

void setupNcurses() {}

void setupFrameworks( AppConfig const& config )
{
    setupRaylib( config );
    setupNcurses();
}

void updateWindowState()
{
    if ( IsKeyPressed( KEY_F11 ) )
    {
        if ( IsWindowMaximized() )
        {
            RestoreWindow();
        }

        else
        {
            MaximizeWindow();
        }
    }

    if ( IsWindowResized() )
    {
        snx::EventDispatcher::notify( EventId::WINDOW_RESIZED );
    }
}

void updateDeveloperMode()
{
    if ( IsKeyPressed( KEY_F1 ) )
    {
        DeveloperMode::toggle();
    }
}

InputId getUserInput( InputHandler& inputHandler )
{
    InputId inputId{};

    //* Take input from mouse, keys or gestures
    inputId = inputHandler.fromKeyboard();

    if ( inputId != InputId::NONE )
    {
        return inputId;
    }

    inputId = inputHandler.fromMouse();

    if ( inputId != InputId::NONE )
    {
        return inputId;
    }

    inputId = inputHandler.fromGesture();

    return inputId;
}

/// @brief C-Style void argument in function signature needed for emscripten
void updateApp( void* arg )
{
    App& app = *(App*)arg;

    updateWindowState();
#if defined( DEBUG )
    updateDeveloperMode();
#endif
    app.currentInputId = getUserInput( app.inputHandler );

    if ( app.currentInputId == InputId::TOGGLE_CURSOR )
    {
        app.inputHandler.toggleCursorState();
        snx::EventDispatcher::notify( EventId::CURSOR_TOGGLE );
    }

    app.dt = GetFrameTime();

    app.cursor = CursorModule::update(
        app.cursor,
        app.scenes.game.gameCamera.camera,
        Convert::worldToTile( app.game.hero.position )
    );

    switch ( app.state )
    {
        default:
        case AppState::RUN_GAME:
        {
            app.game = GameModule::update(
                app.game,
                app.scenes.game.gameCamera,
                app.cursor,
                app.currentInputId,
                app.dt
            );

            SceneGameModule::update(
                app.scenes.game,
                app.game.hero,
                app.game.world,
                app.cursor,
                app.currentInputId
            );
            break;
        }

        case AppState::GAME_OVER:
        {
            app.scenes.gameOver = SceneGameOverModule::update( app.scenes.gameOver );

            break;
        }
    }
}

void setupAppEvents( App& app )
{
#if defined( DEBUG ) && defined( DEBUG_TILEINFO )
    snx::EventDispatcher::addListener(
        EventId::CURSOR_POSITION_CHANGED,
        [&]()
        {
            Vector2I cursorPos{ Convert::worldToTile( app.cursor.position ) };

            Tiles& tiles{ app.game.world.currentMap->tiles };

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

            Objects& objects{ app.game.world.currentMap->objects };

            if ( objects.ids.contains( Convert::worldToTile( app.cursor.position ) ) )
            {
                size_t objectIdx{ objects.ids.index( Convert::worldToTile( app.cursor.position ) ) };

                snx::debug::cliLog( "OBJECT\n" );

                snx::debug::cliPrint(
                    "Name: "
                    + app.game.world.currentMap->objects.names[objectIdx]
                    + "\n"
                );

                snx::debug::cliPrint(
                    "Actions: "
                    + app.game.world.currentMap->objects.actions[objectIdx]
                    + "\n"
                );

                snx::debug::cliPrint(
                    "RenderId: "
                    + std::to_string( static_cast<int>( app.game.world.currentMap->objects.renderIds[objectIdx] ) )
                    + "\n"
                );

                snx::debug::cliPrint(
                    "Event: "
                    + std::to_string( static_cast<int>( app.game.world.currentMap->objects.eventIds[objectIdx] ) )
                    + "\n"
                );
            }

            if ( app.game.world.currentMap->enemies.ids.contains( cursorPos ) )
            {
                snx::debug::cliLog( "ENEMY\n" );

                snx::debug::cliPrint(
                    "Id: "
                    + std::to_string( app.game.world.currentMap->enemies.ids.at( cursorPos ) )
                    + "\n"
                );
            }
        }
    );
#endif
    snx::EventDispatcher::addListener(
        EventId::CURSOR_TOGGLE,
        [&]()
        {
            app.cursor = CursorModule::toggle( app.cursor );
        }
    );

    snx::EventDispatcher::addListener(
        EventId::WINDOW_RESIZED,
        [&]()
        {
            app.scenes.game.panels = PanelSystem::init( app.scenes.game.panels );

            app.scenes.game.gameCamera = GameCameraModule::init(
                app.scenes.game.gameCamera,
                app.scenes.game.panels.map.box(),
                app.game.hero.position
            );

            app.game.world.currentMap->tiles = VisibilitySystem::calculateVisibilities(
                app.game.world.currentMap->tiles,
                app.game.world.currentMap->fogs,
                GameCameraModule::viewportInTiles( app.scenes.game.gameCamera ),
                Convert::worldToTile( app.game.hero.position ),
                app.game.hero.visionRange
            );
        },
        true
    );

    snx::EventDispatcher::addListener(
        EventId::GAME_OVER,
        [&]()
        {
            app.state = AppState::GAME_OVER;
        }
    );
}

namespace AppModule
{
    App const& init(
        App& appIO,
        AppConfig const& config
    )
    {
        setupFrameworks( config );

        appIO.game = GameModule::init( appIO.game );

        appIO.scenes.game = SceneGameModule::init(
            appIO.scenes.game,
            appIO.game.hero,
            appIO.game.world
        );

        appIO.scenes.gameOver = SceneGameOverModule::init( appIO.scenes.gameOver );

        setupAppEvents( appIO );

        return appIO;
    }

    void run( App& appIO )
    {
#if defined( EMSCRIPTEN )
        emscripten_set_main_loop_arg(
            updateApp,
            &appIO,
            60 /*FPS*/,
            1 /*Simulate infinite loop*/
        );
#else
        while ( !( WindowShouldClose() ) )
        {
            updateApp( &appIO );
        }
#endif
    }

    void deinit( App& appIO )
    {
        GameFont::unload();
        SceneGameModule::deinitialize( appIO.scenes.game );

        //* Close window and opengl context
        CloseWindow();
    }
}
