#include "Scenes.h"
#define RAYGUI_IMPLEMENTATION // only define once
#define RAYGUI_CUSTOM_ICONS   // Custom icons set required
#include "../resources/iconset.rgi.h"
#include <iostream>
#include <raygui.h>
#include <raylib.h>

int main(/* int argc, char **argv */)
{
    // Initialization
    //---------------------------------
    const int screenWidth = 450;
    const int screenHeight = 550;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);

    InitWindow(screenWidth, screenHeight, "TITLE");

    SetTargetFPS(60);
    // SetExitKey(0); // deactivate exit on ESC
    //---------------------------------

    Game game;

    // Main app loop
    //---------------------------------
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (IsWindowResized())
        {
            game.Initialize();
        }

        switch (currentScene)
        {
        case GAME:
            game.Update();
            break;

        default:
            break;
        }
    }
    //---------------------------------

    // De-Initialization
    //---------------------------------
    CloseWindow(); // Close window and OpenGL context
    //---------------------------------

    return 0;
}
