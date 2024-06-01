#include "App.h"

#include "DeveloperMode.h"
#include "Event.h"
#include "GameFont.h"
#include "PublisherStatic.h"
#include <raygui.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
// Emscripten compatible app loop
//=====================================
void applicationLoop(void* arg_)
{
    emscriptenArgs* arg = (emscriptenArgs*)arg_;

    arg->activeScene->update();
}
//=====================================
#endif

void App::init()
{
    //* TODO: Import configs from user file

    // Raylib flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (config.vSync())
    {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }

    // Initialize window
    InitWindow(config.windowWidth(), config.windowHeight(), "Roguelike");

    // Raylib Settings
    SetWindowIcon(favicon_);
    SetWindowMinSize(config.windowWidth(), config.windowHeight());
    SetExitKey(KEY_F4);
    SetTargetFPS(fpsTarget_);

    // Fonts
    GameFont::load();
    GuiSetStyle(DEFAULT, TEXT_SIZE, GameFont::fontSize);

    // Scene
    gameScene_.initialize();
}

void updateFullscreen()
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

        snx::Publisher::notify(Event::windowResized);
    }
}
void updateDeveloperMode()
{
    if (IsKeyPressed(KEY_F1))
    {
        DeveloperMode::toggle();
    }
};

void App::run()
{
    // Main app loop
    //=================================
#ifdef __EMSCRIPTEN__
    emscriptenArgs arg{activeScene};
    emscripten_set_main_loop_arg(applicationLoop, &arg, 0, 1);
#else
    while (!(WindowShouldClose()))
    {
        updateFullscreen();
        updateDeveloperMode();

        activeScene_->update();
    }
#endif
    //=================================
}

void App::deinit()
{
    GameFont::unload();
    gameScene_.deinitialize();
    CloseWindow(); // Close window and opengl context
}