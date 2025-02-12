#include "App.h"

#include "DeveloperMode.h"
#include "EventId.h"
#include "GameFont.h"
#include "InputHandler.h"
#include "InputId.h"
#include "PublisherStatic.h"
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
        snx::PublisherStatic::publish( EventId::WINDOW_RESIZED );
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
        snx::PublisherStatic::publish( EventId::CURSOR_TOGGLE );
    }

    app.dt = GetFrameTime();

    SceneModule::update(
        app.scene,
        app.cursor,
        app.currentInputId,
        app.dt
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

        snx::PublisherStatic::addSubscriber(
            EventId::CURSOR_TOGGLE,
            [&]()
            {
                appIO.cursor = CursorModule::toggle( appIO.cursor );
            }
        );

        SceneModule::init(
            appIO.scene,
            appIO.cursor
        );

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
