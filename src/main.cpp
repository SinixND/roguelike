#include "ActiveScene.h"
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

void applicationLoop(); // must not have parameters for emscripten compatibility
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

    // Define scenes
    GameScene game{};

    // Set default scene
    ActiveScene activeScene{game};
    //=================================

    // Main app loop
    //=================================
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(applicationLoop, FPS_TARGET, 1);
#else
    while (!(WindowShouldClose()))
    {
        // Call update function for emscripten compatibility
        applicationLoop();
    }

#endif
    //=================================

    // De-initialization
    //=================================
    game.deinitialize();
    CloseWindow(); // Close window and opengl context
    //=================================

    return 0;
}

void updateActiveScene(snx::IScene* activeScene);

void toggleFullscreen();
void applicationLoop()
{
    // Define scenes
    static GameScene game{};

    // Set default scene
    static ActiveScene activeScene{game};

#ifndef __EMSCRIPTEN__
    // Toggle fullscreen
    //=================================
    toggleFullscreen();
    //=================================
#endif

    updateActiveScene(activeScene());
}

void updateActiveScene(snx::IScene* activeScene)
{
    activeScene->update();
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
