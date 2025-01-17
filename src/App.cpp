#include "App.h"
#include "DeveloperMode.h"
#include "EventId.h"
#include "GameFont.h"
#include "PublisherStatic.h"
#include <raygui.h>
#include <raylib.h>

#if defined( EMSCRIPTEN )
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

void emscriptenLoop( void* arg )
{
    App* app = (App*)arg;

    SceneModule::update( app->scene );
}

void updateWindow()
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

void AppModule::init(
    Scene& scene,
    AppConfig const& config
)
{
    //* TODO: Import configs from user file

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

    //* Scene
    SceneModule::init( scene );
}

void AppModule::update( Scene& scene )
{
#if defined( EMSCRIPTEN )
    emscripten_set_main_loop_arg( emscriptenLoop, &app, 60 /*FPS*/, 1 /*Simulate infinite loop*/ );
#else
    while ( !( WindowShouldClose() ) )
    {
        updateWindow();
        updateDeveloperMode();

        SceneModule::update( scene );
    }
#endif
}

void AppModule::deinit( Scene& scene )
{
    GameFont::unload();
    SceneModule::deinitialize( scene );

    //* Close window and opengl context
    CloseWindow();
}
