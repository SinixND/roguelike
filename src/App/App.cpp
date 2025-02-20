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
