#include "Constants.h"
#include "GameScene.h"
#include "IScene.h"
#include "RNG.h"
#include "RenderID.h"
#include "RuntimeDatabase.h"
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

void applicationLoop();
int main(/* int argc, char **argv */)
{
    // Setup the window
    //=====================================
    constexpr int WINDOW_WIDTH{800};
    constexpr int WINDOW_HEIGHT{450};
    constexpr int FPS_TARGET{245};

    // Flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (VSYNC_ACTIVE)
        SetConfigFlags(FLAG_VSYNC_HINT);

    // Initialize window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Roguelike");

    // Settings
    Image favicon{LoadImage("resources/favicon/favicon.png")};
    SetWindowIcon(favicon);
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetExitKey(KEY_F4);
    SetTargetFPS(FPS_TARGET);

    // Fonts
    dtb::loadFont("resources/fonts/JuliaMono-RegularLatin.ttf");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    //=====================================

    // Application initialization
    //=================================
    // Seed Random number generator
    if (dtb::debugMode())
        RNG::seed(1);

    // Load textures
    dtb::loadTexture(RenderID::NONE, "Empty.png");
    dtb::loadTexture(RenderID::HERO, "Hero.png");
    dtb::loadTexture(RenderID::CURSOR, "Cursor.png");
    dtb::loadTexture(RenderID::WALL, "TileWall.png");
    dtb::loadTexture(RenderID::FLOOR, "TileFloor.png");
    dtb::loadTexture(RenderID::REACHABLE, "TileReachable.png");
    dtb::loadTexture(RenderID::PATH, "TilePath.png");
    dtb::loadTexture(RenderID::ATTACKABLE, "TileAttackable.png");
    dtb::loadTexture(RenderID::SUPPORTABLE, "TileSupportable.png");

    // Define scenes
    GameScene game{};
    game.initialize();

    // Set default scene
    dtb::setActiveScene(game);

    // Setup Camera2D
    dtb::camera() = {
      Vector2Scale(GetDisplaySize(), 0.5f),
      V_NULL,
      0.0f,
      1.0f};
    //=================================

    // Main app loop
    //=================================
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(applicationLoop, FPS_TARGET, 1);
#else
    while (!(WindowShouldClose() || dtb::shouldAppClose()))
    {
        // Call update function for emscripten compatibility
        applicationLoop();
    }

#endif
    //=================================

    // De-initialization
    //=================================
    // Unload fonts
    dtb::unloadFont();

    // Unlaod textures
    dtb::unloadAllTextures();

    // Deinitialize scenes
    game.deinitialize();

    CloseWindow(); // Close window and opengl context
    //=================================

    return 0;
}

void applicationLoop()
{
    // Set window dependent variables
    dtb::camera().offset = {GetRenderWidth() * 0.5f, GetRenderHeight() * 0.5f};

#ifndef __EMSCRIPTEN__
    // Toggle fullscreen
    //=================================
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

#endif
    //=================================

    dtb::activeScene()->update(dtb::debugMode());
}
