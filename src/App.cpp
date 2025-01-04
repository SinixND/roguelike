#include "App.h"
#include "DeveloperMode.h"
#include "EventId.h"
#include "GameFont.h"
#include "PublisherStatic.h"
#include <raygui.h>
#include <raylib.h>

#if defined(EMSCRIPTEN)
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

#if defined(EMSCRIPTEN)
void applicationLoop(void* arg)
{
    emscriptenApp* emsApp = (emscriptenApp*)arg;

    SceneModule::update(*emsApp->scene);
}
#endif

void AppModule::init(App& app)
{
    //* TODO: Import configs from user file

    //* Raylib flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (app.config.vSync)
    {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }

    //* Initialize window
    InitWindow(app.config.windowWidth, app.config.windowHeight, "Roguelike");

    //* Raylib Settings
    SetWindowIcon(app.favicon);
    SetWindowMinSize(640, 480);
#if defined(EMSCRIPTEN)
    MaximizeWindow();
#endif
    SetExitKey(KEY_F4);
    SetTargetFPS(app.fpsTarget);

    //* Fonts
    GameFont::load();
    GuiSetStyle(DEFAULT, TEXT_SIZE, GameFont::fontHeight);

    //* Scene
    SceneModule::init(app.scene);
}

void updateWindow()
{
    if (IsKeyPressed(KEY_F11))
    {
        if (IsWindowMaximized())
        {
            RestoreWindow();
        }

        else
        {
            MaximizeWindow();
        }
    }

    if (IsWindowResized())
    {
        snx::PublisherStatic::publish(EventId::WINDOW_RESIZED);
    }
}

void updateDeveloperMode()
{
    if (IsKeyPressed(KEY_F1))
    {
        DeveloperMode::toggle();
    }
}

void AppModule::run(App& app)
{
#if defined(EMSCRIPTEN)
    emscriptenApp emsApp{&app.scene};
    emscripten_set_main_loop_arg(applicationLoop, &emsApp, 60 /*FPS*/, 1 /*Simulate infinite loop*/);
#else
    while (!(WindowShouldClose()))
    {
        updateWindow();
        updateDeveloperMode();

        SceneModule::update(app.scene);
    }
#endif
}

void AppModule::deinit(App& app)
{
    GameFont::unload();
    SceneModule::deinitialize(app.scene);

    //* Close window and opengl context
    CloseWindow();
}
