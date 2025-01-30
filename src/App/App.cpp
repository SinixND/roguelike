#include "App.h"

#include "DeveloperMode.h"
#include "EventId.h"
#include "GameFont.h"
#include "InputHandler.h"
#include "PublisherStatic.h"
#include <raygui.h>
#include <raylib.h>

#if defined( EMSCRIPTEN )
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

/// @brief A separate game loop function needed for emscripten
void emscriptenLoop( void* arg )
{
    App* app = (App*)arg;

    SceneModule::update(
        app->scene,
        app->cursor,
        app->currentInputId,
        app->dt
    );
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

void setupRaylib(
    AppConfig const& config
)
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

namespace AppModule
{
    App const& init(
        App& app,
        AppConfig const& config
    )
    {
        setupFrameworks( config );

        snx::PublisherStatic::addSubscriber(
            EventId::CURSOR_TOGGLE,
            [&]()
            {
                CursorModule::toggle( app.cursor );
            }
        );

        SceneModule::init(
            app.scene,
            app.cursor
        );

        return app;
    }

    void run( App& app )
    {
#if defined( EMSCRIPTEN )
        emscripten_set_main_loop_arg(
            emscriptenLoop,
            &app,
            60 /*FPS*/,
            1 /*Simulate infinite loop*/
        );
#else
        while ( !( WindowShouldClose() ) )
        {
            updateWindowState();
#if defined( DEBUG )
            updateDeveloperMode();
#endif

            app.currentInputId = getUserInput( app.inputHandler );

            app.dt = GetFrameTime();

            SceneModule::update(
                app.scene,
                app.cursor,
                app.currentInputId,
                app.dt
            );
        }
#endif
    }

    void deinit( App& app )
    {
        GameFont::unload();
        SceneModule::deinitialize( app.scene );

        //* Close window and opengl context
        CloseWindow();
    }
}

