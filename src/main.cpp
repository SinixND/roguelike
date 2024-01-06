#include "ACTIVE_SCENE.h"
#include "CONFIGS.h"
#include "Game.h"
#include "Scene.h"
#include "TEXTURE_MANAGER.h"
#define RAYGUI_IMPLEMENTATION // Only define once
#define RAYGUI_CUSTOM_ICONS   // Custom icons set required
#include "../resources/iconset/iconset.rgi.h"
#include "raygui.h"
#include "raylib.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

snd::CONFIGS* configs{snd::CONFIGS::getInstance()};
snd::TEXTURE_MANAGER* textureManager{snd::TEXTURE_MANAGER::getInstance()};
snd::ACTIVE_SCENE* activeScene{snd::ACTIVE_SCENE::getInstance()};

void updateGameLoop();
int main(/* int argc, char **argv */)
{
    // General Initialization
    //=================================

    // Raylib
#ifndef __EMSCRIPTEN__
    constexpr int screenWidth{0};
    constexpr int screenHeight{0};
#else
    std::cout << "SET EMSCRIPTEN WINDOW SIZE 500, 500\n";
    constexpr int screenWidth{500};
    constexpr int screenHeight{500};
#endif

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);

#ifndef __EMSCRIPTEN__
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
#endif

    InitWindow(screenWidth, screenHeight, "Roguelike");

#ifdef __EMSCRIPTEN__
    SetWindowSize(GetRenderWidth() - 50, GetRenderHeight() - 50);
#endif

    HideCursor();

    //  SetExitKey(0); // Deactivate exit on ESC
    SetTargetFPS(145);
    // SetConfigFlags(FLAG_VSYNC_HINT); // Used instead of SetTargetFPS()
    //=================================

    // Application Initialization
    //=================================

    // Load textures
    textureManager->loadTexture(PLAYER, "Player.png");
    textureManager->loadTexture(CURSOR, "Cursor.png");

    // Define scenes
    snd::GameScene game{};
    game.initialize();

    // Set default scene
    activeScene->setScene(game);
    //=================================

    // Main app loop
    //=================================
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(updateGameLoop, 145, 1);
#else
    while (!WindowShouldClose() && !configs->shouldAppClose())
    {
        updateGameLoop();
    }
#endif
    //=================================

    // De-Initialization
    //=================================
    // Unlaod textures
    textureManager->unloadAllTextures();

    // Deinitialize scenes
    game.deinitialize();

    CloseWindow(); // Close window and OpenGL context
    //=================================

    return 0;
}

void updateGameLoop()
{
    activeScene->getScene().update();
}