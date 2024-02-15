#include "DirectionVector.h"
#include "Game.h"
#include "RuntimeDatabase.h"
#include "Scene.h"
#include <iostream>

#define RAYGUI_IMPLEMENTATION // Only define once
#define RAYGUI_CUSTOM_ICONS   // Custom icons set required
#include "../resources/iconset/iconset.rgi.h"

#include <raygui.h>
#include <raylibEx.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#ifndef __EMSCRIPTEN__
constexpr int WINDOW_WIDTH{800};
constexpr int WINDOW_HEIGHT{450};
#else
constexpr int windowWidth{800};
constexpr int windowHeight{450};
#endif

void applicationLoop();
int main(/* int argc, char **argv */)
{
    // Setup the window
    //=====================================
    // Flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (dtb::Configs::vSyncMode())
        SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Roguelike");

    // Settings
    Image favicon{LoadImage("resources/favicon/favicon.png")};
    SetWindowIcon(favicon);
    SetWindowMinSize(320, 240);
    SetExitKey(KEY_F4);
    SetTargetFPS(245);

    // Fonts
    dtb::Constants::loadFont("resources/fonts/JuliaMono-RegularLatin.ttf");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    //=====================================

    // Application Initialization
    //=================================
    // Load textures
    dtb::Textures::load(RENDER_HERO, "Hero.png");
    dtb::Textures::load(RENDER_CURSOR, "Cursor.png");
    dtb::Textures::load(RENDER_WALL_TILE, "TileWall.png");
    dtb::Textures::load(RENDER_FLOOR_TILE, "TileFloor.png");
    dtb::Textures::load(RENDER_REACHABLE_TILE, "TileReachable.png");
    dtb::Textures::load(RENDER_PATH_TILE, "TilePath.png");
    dtb::Textures::load(RENDER_ATTACKABLE_TILE, "TileAttackable.png");
    dtb::Textures::load(RENDER_SUPPORTABLE_TILE, "TileSupportable.png");

    // Define scenes
    GameScene game{};
    game.initialize();

    // Set default scene
    dtb::ActiveScene::setScene(game);

    // Setup Camera2D
    dtb::Globals::setCamera(
        {Vector2Scale(GetDisplaySize(), 0.5f),
         V_NULL,
         0.0f,
         1.0f});
//=================================

// Main app loop
//=================================
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(updateGameLoop, 245, 1);
#else
    while (!WindowShouldClose() && !dtb::Globals::shouldAppClose())
    {
        // Call update function for emscripten compatibility
        applicationLoop();
    }
#endif
    //=================================

    // De-Initialization
    //=================================
    // Unload fonts
    dtb::Constants::unloadFont();

    // Unlaod textures
    dtb::Textures::unloadAll();

    // Deinitialize scenes
    game.deinitialize();

    CloseWindow(); // Close window and OpenGL context
    //=================================

    return 0;
}

void applicationLoop()
{
    dtb::Globals::setCameraOffset({GetRenderWidth() * 0.5f, GetRenderHeight() * 0.5f});

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

    dtb::ActiveScene::scene().update();
}
