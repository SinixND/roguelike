#include "ACTIVE_SCENE.h"
#include "CONFIGS.h"
#include "Game.h"
#include "Scene.h"
#include "TEXTURE_MANAGER.h"
#define RAYGUI_IMPLEMENTATION // Only define once
#define RAYGUI_CUSTOM_ICONS   // Custom icons set required
#include "../resources/iconset/iconset.rgi.h"
#include "raygui.h"
#include <raylib.h>

int main(/* int argc, char **argv */)
{
    // General Initialization
    //=================================

    // Raylib
    constexpr int screenWidth = 0;
    constexpr int screenHeight = 0;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);

    InitWindow(screenWidth, screenHeight, "Roguelike");

    HideCursor();

    //  SetExitKey(0); // Deactivate exit on ESC
    SetTargetFPS(145);
    // SetConfigFlags(FLAG_VSYNC_HINT); // Used instead of SetTargetFPS()
    //=================================

    // Application Initialization
    //=================================
    snd::CONFIGS* configs{snd::CONFIGS::getInstance()};

    // Load textures
    snd::TEXTURE_MANAGER* textureManager{snd::TEXTURE_MANAGER::getInstance()};

    textureManager->loadTexture(PLAYER, "Player.png");
    textureManager->loadTexture(CURSOR, "Cursor.png");

    // Define scenes
    snd::GameScene game{};
    game.initialize();

    // Set default scene
    snd::ACTIVE_SCENE* activeScene{snd::ACTIVE_SCENE::getInstance()};
    activeScene->setScene(game);
    //=================================

    // Main app loop
    //=================================
    while (!WindowShouldClose() && !configs->shouldAppClose()) // Detect window close button or ESC key
    {
        activeScene->getScene().update();
    }
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