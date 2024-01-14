#include "Scene.h"
#include "CONSTANTS.h"
#include "raylib.h"

namespace snd
{
    void Scene::update()
    {
        processInput();
        updateState();

        BeginDrawing();

        ClearBackground(CONSTANTS::get().getBackgroundColor());

        // Draw simple frame
        DrawRectangleLinesEx(Rectangle{0, 0, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())}, BORDER_WEIGHT, BORDER_COLOR);

        if (CONFIGS::get().getDebugMode())
        {
            DrawFPS(0, 0);
        }

        renderOutput();

        EndDrawing();
    };
}