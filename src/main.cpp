#include "FontProperties.h"
#include "GameScene.h"
#include "IScene.h"
#include "VSync.h"
#include <raylib.h>
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION // Only define once
#define RAYGUI_CUSTOM_ICONS   // Custom icons set required
#include "../resources/iconset/iconset.rgi.h"

#include <raygui.h>
#include <raylibEx.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
// Emscripten compatible app loop
//=====================================
struct emscriptenArgs
{
    snx::IScene* activeScene;
};

void applicationLoop(void* arg_)
{
    emscriptenArgs* arg = (emscriptenArgs*)arg_;

    arg->activeScene->update();
}
//=====================================
#endif

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

int main(/* int argc, char **argv */)
{
    // Setup the window
    //=====================================
    constexpr int WINDOW_WIDTH{800};
    constexpr int WINDOW_HEIGHT{450};
    constexpr int FPS_TARGET{245};

    // Flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (VSync::VSYNC_ACTIVE)
    {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }

    // Initialize window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Roguelike");

    // Raylib Settings
    Image favicon{LoadImage("resources/favicon/favicon.png")};
    SetWindowIcon(favicon);
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetExitKey(KEY_F4);
    SetTargetFPS(FPS_TARGET);

    // Fonts
    LoadFont("resources/fonts/JuliaMono-RegularLatin.ttf");
    GuiSetStyle(DEFAULT, TEXT_SIZE, FontProperties::FONT_SIZE);
    //=====================================

    // Application initialization
    //=================================

    // Setup scenes
    GameScene gameScene{};

    // Set default scene
    snx::IScene* activeScene{&gameScene};

    // Set emscripten argument
#ifdef __EMSCRIPTEN__
    emscriptenArgs arg{activeScene};
#endif
    //=================================

    // Main app loop
    //=================================
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(applicationLoop, &arg, 0, 1);
#else
    while (!(WindowShouldClose()))
    {
        // Toggle fullscreen
        toggleFullscreen();

        activeScene->update();
    }
#endif
    //=================================

    // De-initialization
    //=================================
    gameScene.deinitialize();
    CloseWindow(); // Close window and opengl context
    //=================================

    return 0;
}
