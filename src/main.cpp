#include "ActiveScene.h"
#include "Configs.h"
#include "Game.h"
#include "Scene.h"
#include "TextureManager.h"
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
    // Constexpr int screenWidth = 620;
    // Constexpr int screenHeight = 480;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    SetConfigFlags(FLAG_WINDOW_MAXIMIZED);

    // InitWindow(screenWidth, screenHeight, "roguelike");
    InitWindow(0, 0, "Roguelike");

    //  SetExitKey(0); // Deactivate exit on ESC
    SetTargetFPS(145);
    // SetConfigFlags(FLAG_VSYNC_HINT); // Used instead of SetTargetFPS()
    //=================================

    // Application Initialization
    //=================================
    snd::Configs* configs{
        snd::Configs::getInstance()};

    // Load textures
    snd::TextureManager* textureManager{
        snd::TextureManager::getInstance()};

    textureManager->loadTexture(PLAYER, "Player.png");

    // Define scenes
    snd::GameScene game{};
    game.initialize();

    // Set default scene
    snd::ActiveScene* activeScene{snd::ActiveScene::getInstance()};
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