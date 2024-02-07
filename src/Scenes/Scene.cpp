#include "Scene.h"

#include "RuntimeDatabase.h"
#include <raylib.h>
#include <raylibEx.h>

namespace snd
{
    constexpr float BORDER_WEIGHT{1};
    constexpr Color BORDER_COLOR{GRAY};
    constexpr Color BACKGROUND_COLOR{BLACK};

    void Scene::update()
    {
        processInput();
        updateState();

        BeginDrawing();

        ClearBackground(BACKGROUND_COLOR);

        if (dtb::Configs::getDebugMode())
        {
            DrawFPS(0, 0);
        }

        BeginMode2D(dtb::Globals::getCamera());

        renderOutput();

        EndMode2D();

        // Draw simple frame
        DrawRectangleLinesEx(Rectangle{0, 0, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())}, BORDER_WEIGHT, BORDER_COLOR);

        EndDrawing();
    };
}
