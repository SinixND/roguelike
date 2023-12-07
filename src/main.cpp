#include "CurrentScene.h"
#include "Game.h"
#include "Scene.h"
#define RAYGUI_IMPLEMENTATION // only define once
#define RAYGUI_CUSTOM_ICONS   // Custom icons set required
#include "../resources/iconset.rgi.h"
#include <iostream>
#include <raygui.h>
#include <raylib.h>

int main(/* int argc, char **argv */)
{
    // General Initialization
    //---------------------------------

    // Raylib
    constexpr int screenWidth = 620;
    constexpr int screenHeight = 480;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    SetConfigFlags(FLAG_VSYNC_HINT); // used instead of SetTargetFPS()

    InitWindow(screenWidth, screenHeight, "roguelike");

    MaximizeWindow();
    // SetExitKey(0); // deactivate exit on ESC
    //---------------------------------

    // Application Initialize
    //---------------------------------
    // Define scenes
    snd::Game game{};
    game.initialize();

    // Set default scene
    snd::CurrentScene* currentScene{snd::CurrentScene::getInstance()};
    currentScene->setScene(game);
    //---------------------------------

    // Main app loop
    //---------------------------------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (IsWindowResized())
        {
            currentScene->getScene().initialize();
        }

        currentScene->getScene().update();
    }
    //---------------------------------

    // De-Initialization
    //---------------------------------
    CloseWindow(); // Close window and OpenGL context
    //---------------------------------

    return 0;
}