#include "Game.h"
#include "RuntimeDatabase.h"
#include "Scene.h"
#include "Utility.h"
#include <raylib.h>
#include <raylibEx.h>
#define RAYGUI_IMPLEMENTATION // Only define once
#define RAYGUI_CUSTOM_ICONS   // Custom icons set required
#include "../resources/iconset/iconset.rgi.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

void applicationLoop(Camera2D camera = {});
int main(/* int argc, char **argv */)
{
    // Setup the window
    //=====================================
#ifndef __EMSCRIPTEN__
    constexpr int windowWidth{0};
    constexpr int windowHeight{0};
#else
    constexpr int windowWidth{800};
    constexpr int windowHeight{450};
#endif

    // Flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (dtb::Configs::getVSyncMode())
        SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(windowWidth, windowHeight, "Roguelike");

    // Settings
    Image favicon{LoadImage("resources/favicon/favicon.png")};
    SetWindowIcon(favicon);
    SetWindowMinSize(320, 240);
    SetExitKey(KEY_F4);
    SetTargetFPS(245);
    //=====================================

    // Application Initialization
    //=================================
    // Load textures
    dtb::Textures::load(PLAYER, "Player.png");
    dtb::Textures::load(CURSOR, "Cursor.png");
    dtb::Textures::load(WALL_TILE, "TileWall.png");
    dtb::Textures::load(FLOOR_TILE, "TileFloor.png");
    dtb::Textures::load(REACHABLE_TILE, "TileReachable.png");
    dtb::Textures::load(PATH_TILE, "TilePath.png");
    dtb::Textures::load(ATTACKABLE_TILE, "TileAttackable.png");
    dtb::Textures::load(SUPPORTABLE_TILE, "TileSupportable.png");

    // Define scenes
    GameScene game{};
    game.initialize();

    // Set default scene
    dtb::ActiveScene::setScene(game);

    // Setup Camera2D
    Camera2D camera{Vector2Scale(GetDisplaySize(), 0.5f), V_NULL, 0.0f, 1.0f};
    //=================================

    // Main app loop
    //=================================
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(updateGameLoop, 245, 1);
#else
    while (!WindowShouldClose() && !dtb::State::shouldAppClose())
    {
        // Call update function for emscripten compatibility
        applicationLoop(camera);
    }
#endif
    //=================================

    // De-Initialization
    //=================================
    // Unlaod textures
    dtb::Textures::unloadAll();

    // Deinitialize scenes
    game.deinitialize();

    CloseWindow(); // Close window and OpenGL context
    //=================================

    return 0;
}

void applicationLoop(Camera2D camera)
{
    dtb::ActiveScene::getScene().update(camera);
}
