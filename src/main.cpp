#include "Directions.h"
#include "FontProperties.h"
#include "GameScene.h"
#include "IScene.h"
#include "RNG.h"
#include "RenderID.h"
#include "RuntimeDatabase.h"
#include "VSync.h"
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

void applicationLoop(); // must not have parameters for emscripten compatibility
int main(/* int argc, char **argv */)
{
    // Setup the window
    //=====================================
    constexpr int WINDOW_WIDTH{800};
    constexpr int WINDOW_HEIGHT{450};
    constexpr int FPS_TARGET{245};

    // Flags
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    if (VSync::VSYNC_ACTIVE)
    {
        SetConfigFlags(FLAG_VSYNC_HINT);
    }

    // Initialize window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Roguelike");

    // Raylib Settings
    //* Image favicon{LoadImage("resources/favicon/favicon.png")};
    Image favicon{LoadImage("resources/favicon/favicon.png")};
    SetWindowIcon(favicon);
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetExitKey(KEY_F4);
    SetTargetFPS(FPS_TARGET);

    // Fonts
    dtb::loadFont("resources/fonts/JuliaMono-RegularLatin.ttf");
    GuiSetStyle(DEFAULT, TEXT_SIZE, FontProperties::FONT_SIZE);
    //=====================================

    // Application initialization
    //=================================
    // Seed Random number generator
    if (dtb::debugMode())
    {
        snx::RNG::seed(1);
    }

    // Load texture atlas
    dtb::loadAtlas("TextureAtlas.png");

    // Register textures
    dtb::registerTexture(RenderID::INVISIBLE, {0, 0});
    dtb::registerTexture(RenderID::CURSOR, {35, 0});
    dtb::registerTexture(RenderID::HERO, {70, 0});
    dtb::registerTexture(RenderID::WALL, {105, 0});
    dtb::registerTexture(RenderID::FLOOR, {0, 35});
    dtb::registerTexture(RenderID::REACHABLE, {35, 35});
    dtb::registerTexture(RenderID::PATH, {70, 35});
    dtb::registerTexture(RenderID::ATTACKABLE, {105, 35});
    dtb::registerTexture(RenderID::SUPPORTABLE, {0, 70});
    dtb::registerTexture(RenderID::NEXT_LEVEL, {35, 70});

    // Define scenes
    GameScene game{};
    game.initialize();

    // Set default scene
    dtb::setActiveScene(game);

    // Setup Camera2D
    dtb::camera() = {
        Vector2Scale(GetDisplaySize(), 0.5f),
        Directions::V_NULL,
        0,
        1};
    //=================================

    // Main app loop
    //=================================
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(applicationLoop, FPS_TARGET, 1);
#else
    while (!(WindowShouldClose() || dtb::shouldAppClose()))
    {
        // Call update function for emscripten compatibility
        applicationLoop();
    }

#endif
    //=================================

    // De-initialization
    //=================================
    // Unload fonts
    dtb::unloadFont();

    // Unload texture atlas
    dtb::unloadAtlas();

    // Deinitialize scenes
    game.deinitialize();

    CloseWindow(); // Close window and opengl context
    //=================================

    return 0;
}

void updateCameraOffset(Camera2D& camera);
void updateActiveScene(snx::IScene* activeScene, bool debugMode);
void applicationLoop()
{
    // Set window dependent variables
    updateCameraOffset(dtb::camera());

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

    updateActiveScene(dtb::activeScene(), dtb::debugMode());
}

void updateCameraOffset(Camera2D& camera)
{
    camera.offset = {static_cast<int>(GetRenderWidth()) * 0.5F, GetRenderHeight() * 0.5F};
}

void updateActiveScene(snx::IScene* activeScene, bool debugMode)
{
    activeScene->update(debugMode);
}