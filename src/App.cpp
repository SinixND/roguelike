#include "App.h"
#include "DeveloperMode.h"
#include "EventId.h"
#include "GameFont.h"
#include "PublisherStatic.h"
#include <raygui.h>
#include <raylib.h>

#if defined(EMSCRIPTEN)
#include "Logger.h"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <string>
#endif

#if defined(EMSCRIPTEN)
void applicationLoop(void& arg_)
{
    emscriptenArgs& arg = (emscriptenArgs*)arg_;

    arg.activeScene->update();
}
#endif

void App::init()
{
    //* TODO: Import configs from user file

    //* Raylib flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (config_.vSync())
    {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }

    //* Initialize window
    InitWindow(config_.windowWidth(), config_.windowHeight(), "Roguelike");

    //* Raylib Settings
    SetWindowIcon(favicon_);
    SetWindowMinSize(640, 480);
#if defined(EMSCRIPTEN)
    MaximizeWindow();
#endif
    SetExitKey(KEY_F4);
    SetTargetFPS(fpsTarget_);

    //* Fonts
    GameFont::load();
    GuiSetStyle(DEFAULT, TEXT_SIZE, GameFont::fontHeight);

    //* Scene
    SceneMain_.init();
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

void App::run()
{
#if defined(EMSCRIPTEN)
    emscriptenArgs arg{activeScene_};
    emscripten_set_main_loop_arg(applicationLoop, &arg, 60 /*FPS*/, 1 /*Simulate infinite loop*/);
#else
    while (!(WindowShouldClose()))
    {
        updateWindow();
        updateDeveloperMode();

        activeScene_->run();
    }
#endif
}

void App::deinit()
{
    GameFont::unload();
    SceneMain_.deinitialize();

    //* Close window and opengl context
    CloseWindow();
}
