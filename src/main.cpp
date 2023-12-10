#include "ActiveScene.h"
#include "Game.h"
#include "Scene.h"
#define RAYGUI_IMPLEMENTATION // only define once
#define RAYGUI_CUSTOM_ICONS   // Custom icons set required
#include "../resources/iconset.rgi.h"
#include "raygui.h"
#include <iostream>
#include <raylib.h>

int main(/* int argc, char **argv */)
{
    // General Initialization
    //---------------------------------

    // Raylib
    // constexpr int screenWidth = 620;
    // constexpr int screenHeight = 480;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    SetConfigFlags(FLAG_VSYNC_HINT); // used instead of SetTargetFPS()
    SetConfigFlags(FLAG_WINDOW_MAXIMIZED);

    // InitWindow(screenWidth, screenHeight, "roguelike");
    InitWindow(0, 0, "roguelike");

    // MaximizeWindow();
    //  SetExitKey(0); // deactivate exit on ESC
    //---------------------------------

    // Application Initialize
    //---------------------------------
    // Define scenes
    snd::GameScene game{};
    game.initialize();

    // Set default scene
    snd::ActiveScene* activeScene{snd::ActiveScene::getInstance()};
    activeScene->setScene(game);
    //---------------------------------

    // Main app loop
    //---------------------------------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (IsWindowResized())
        {
            activeScene->getScene().initialize();
        }

        activeScene->getScene().update();
    }
    //---------------------------------

    // De-Initialization
    //---------------------------------
    CloseWindow(); // Close window and OpenGL context
    //---------------------------------

    return 0;
}