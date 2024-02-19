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

constexpr int WINDOW_WIDTH{800};
constexpr int WINDOW_HEIGHT{450};

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
    dtb::Textures::load(RenderID::none, "Empty.png");
    dtb::Textures::load(RenderID::hero, "Hero.png");
    dtb::Textures::load(RenderID::cursor, "Cursor.png");
    dtb::Textures::load(RenderID::wall, "TileWall.png");
    dtb::Textures::load(RenderID::floor, "TileFloor.png");
    dtb::Textures::load(RenderID::reachable, "TileReachable.png");
    dtb::Textures::load(RenderID::path, "TilePath.png");
    dtb::Textures::load(RenderID::attackable, "TileAttackable.png");
    dtb::Textures::load(RenderID::supportable, "TileSupportable.png");

    // Define scenes
    GameScene game{};
    game.initialize();

    // Set default scene
    dtb::ActiveScene::setScene(game);

    // Setup Camera2D
    dtb::Globals::camera() =
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
    // Set window dependent variables
    dtb::Globals::camera().offset = {GetRenderWidth() * 0.5f, GetRenderHeight() * 0.5f};

    dtb::Constants::setCursorDeadzone(
        GetRectangle(
            Vector2AddValue(
                {0, 0},
                DEADZONE_PIXELS),
            Vector2SubtractValue(
                {static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())},
                DEADZONE_PIXELS)));

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
