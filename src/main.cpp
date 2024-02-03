#include "Game.h"
#include "RuntimeDatabase.h"
#include "Scene.h"
#define RAYGUI_IMPLEMENTATION // Only define once
#define RAYGUI_CUSTOM_ICONS   // Custom icons set required
#include "../resources/iconset/iconset.rgi.h"
#include <raygui.h>
#include <raylibEx.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

void updateGameLoop();
int main(/* int argc, char **argv */)
{
// General Initialization
//=================================

// Raylib
#ifndef __EMSCRIPTEN__
    constexpr int windowWidth{0};
    constexpr int windowHeight{0};
#else
    constexpr int windowWidth{800};
    constexpr int windowHeight{450};
#endif

    SetConfigFlags(FLAG_VSYNC_HINT);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(windowWidth, windowHeight, "Roguelike");
    SetWindowMinSize(320, 240);
    //=================================

    // Application Initialization
    //=================================

    // Load textures
    dtb::Textures::load(PLAYER_TEXTURE, "Player.png");
    dtb::Textures::load(CURSOR_TEXTURE, "Cursor.png");
    dtb::Textures::load(WALL_TEXTURE, "TileWall.png");
    dtb::Textures::load(FLOOR_TEXTURE, "TileFloor.png");
    dtb::Textures::load(REACHABLE_TILE, "TileReachable.png");
    dtb::Textures::load(ATTACKABLE_TILE, "TileAttackable.png");
    dtb::Textures::load(SUPPORTABLE_TILE, "TileSupportable.png");

    // Define scenes
    GameScene game{};
    game.initialize();

    // Set default scene
    dtb::ActiveScene::setScene(game);
    //=================================

    // Main app loop
    //=================================
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(updateGameLoop, 145, 1);
#else
    while (!WindowShouldClose() && !dtb::Configs::shouldAppClose())
    {
        // Call update function for emscripten compatibility
        updateGameLoop();
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

void updateGameLoop()
{
    dtb::ActiveScene::getScene().update();
}
