#include "Constants.h"
#include "Game.h"
#include "RuntimeDatabase.h"
#include "Scene.h"
#include "TilePositionConversion.h"
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

    // Flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (VSYNC_ACTIVE)
        SetConfigFlags(FLAG_VSYNC_HINT);

    // Initialize window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Roguelike");

    // Settings
    Image favicon{LoadImage("resources/favicon/favicon.png")};
    SetWindowIcon(favicon);
    SetWindowMinSize(320, 240);
    SetExitKey(KEY_F4);
    SetTargetFPS(245);

    // Fonts
    Font font{LoadFont("resources/fonts/JuliaMono-RegularLatin.ttf")};
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    //=====================================

    // Application Initialization
    //=================================
    // Load textures
    dtb::loadTexture(RenderID::none, "Empty.png");
    dtb::loadTexture(RenderID::hero, "Hero.png");
    dtb::loadTexture(RenderID::cursor, "Cursor.png");
    dtb::loadTexture(RenderID::wall, "TileWall.png");
    dtb::loadTexture(RenderID::floor, "TileFloor.png");
    dtb::loadTexture(RenderID::reachable, "TileReachable.png");
    dtb::loadTexture(RenderID::path, "TilePath.png");
    dtb::loadTexture(RenderID::attackable, "TileAttackable.png");
    dtb::loadTexture(RenderID::supportable, "TileSupportable.png");

    // Define scenes
    GameScene game{};
    game.initialize();

    // Set default scene
    dtb::setActiveScene(game);

    // Setup Camera2D
    dtb::camera() =
        {Vector2Scale(GetDisplaySize(), 0.5f),
         V_NULL,
         0.0f,
         1.0f};
    //=================================

    // Main app loop
    //=================================
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(applicationLoop, 245, 1);
#else
    while (!(WindowShouldClose() || dtb::shouldAppClose()))
    {
        // Call update function for emscripten compatibility
        applicationLoop();
    }
#endif
    //=================================

    // De-Initialization
    //=================================
    // Unload fonts
    UnloadFont(font);

    // Unlaod textures
    dtb::unloadAllTextures();

    // Deinitialize scenes
    game.deinitialize();

    CloseWindow(); // Close window and OpenGL context
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

    dtb::activeScene().update();
}
