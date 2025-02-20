#include "App.h"

#include "DeveloperMode.h"
#include "EventDispatcher.h"
#include "Events.h"
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

#if defined( DEBUG )
#include "RNG.h"
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

    //* Fonts
    GameFont::load();
    GuiSetStyle( DEFAULT, TEXT_SIZE, GameFont::fontHeight );
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

/// @brief Exact function signature needed for emscripten
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
        app.scene.gameCamera.camera,
        Convert::worldToTile( app.game.hero.position )
    );

    app.game = GameModule::update(
        app.game,
        app.scene.gameCamera,
        app.cursor,
        app.currentInputId,
        app.dt
    );

    app.scene = SceneModule::update(
        app.scene,
        app.game.hero,
        app.game.levels,
        app.cursor,
        app.currentInputId
    );
}

void setupAppEvents( App& app )
{
#if defined( DEBUG ) && defined( DEBUG_TILEINFO )
    snx::EventDispatcher::addListener(
        EventId::CURSOR_POSITION_CHANGED,
        [&]()
        {
            Vector2I cursorPos{ Convert::worldToTile( cursor.position ) };

            Tiles& tiles{ levels.currentMap->tiles };

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

            Objects& objects{ levels.currentMap->objects };

            if ( objects.ids.contains( Convert::worldToTile( cursor.position ) ) )
            {
                size_t objectId{ objects.ids.at( Convert::worldToTile( cursor.position ) ) };

                snx::debug::cliLog( "OBJECT\n" );

                snx::debug::cliPrint(
                    "Name: "
                    + levels.currentMap->objects.names.at( objectId )
                    + "\n"
                );

                snx::debug::cliPrint(
                    "Actions: "
                    + levels.currentMap->objects.actions.at( objectId )
                    + "\n"
                );

                snx::debug::cliPrint(
                    "RenderId: "
                    + std::to_string( static_cast<int>( levels.currentMap->objects.renderIds.at( objectId ) ) )
                    + "\n"
                );

                snx::debug::cliPrint(
                    "Event: "
                    + std::to_string( static_cast<int>( levels.currentMap->objects.events.at( objectId ) ) )
                    + "\n"
                );
            }

            if ( levels.currentMap->enemies.ids.contains( cursorPos ) )
            {
                snx::debug::cliLog( "ENEMY\n" );

                snx::debug::cliPrint(
                    "Id: "
                    + std::to_string( levels.currentMap->enemies.ids.at( cursorPos ) )
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
            app.scene.panels = PanelSystem::init( app.scene.panels );

            app.scene.gameCamera = GameCameraModule::init(
                app.scene.gameCamera,
                app.scene.panels.map,
                app.game.hero.position
            );

            app.game.levels.currentMap->tiles = VisibilitySystem::calculateVisibilities(
                app.game.levels.currentMap->tiles,
                app.game.levels.currentMap->fogs,
                GameCameraModule::viewportInTiles( app.scene.gameCamera ),
                Convert::worldToTile( app.game.hero.position ),
                app.game.hero.visionRange
            );
        },
        true
    );
}

namespace AppModule
{
    App const& init(
        App& appIO,
        AppConfig const& config
    )
    {
#if defined( DEBUG )
        snx::RNG::seed( 1 );
#endif
        setupFrameworks( config );

        appIO.game = GameModule::init( appIO.game );

        appIO.scene = SceneModule::init(
            appIO.scene,
            appIO.game.hero,
            appIO.game.levels,
            appIO.cursor
        );

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
        SceneModule::deinitialize( appIO.scene );

        //* Close window and opengl context
        CloseWindow();
    }
}
