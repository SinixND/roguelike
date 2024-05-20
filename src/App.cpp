#include "App.h"
#include "FontProperties.h"
#include "VSync.h"
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
    // Raylib flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (VSync::VSYNC_ACTIVE)
    {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }

    // Initialize window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Roguelike");

    // Raylib Settings
    SetWindowIcon(favicon);
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetExitKey(KEY_F4);
    SetTargetFPS(FPS_TARGET);

    // Fonts
    LoadFont("resources/fonts/JuliaMono-RegularLatin.ttf");
    GuiSetStyle(DEFAULT, TEXT_SIZE, FontProperties::FONT_SIZE);

    // Scene
    gameScene.initialize();
}

void toggleFullscreen()
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
}

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
        toggleFullscreen();

        activeScene->update();
    }
#endif
    //=================================
}

void App::deinit()
{
    gameScene.deinitialize();
    CloseWindow(); // Close window and opengl context
}