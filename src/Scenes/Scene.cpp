#include "Scene.h"

#include "RuntimeDatabase.h"
#include <raylib.h>

namespace snd
{
    void Scene::update()
    {
        processInput();
        updateState();

        BeginDrawing();

        ClearBackground(dtb::Constants::backgroundColor_);

        // Draw simple frame
        DrawRectangleLinesEx(Rectangle{0, 0, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())}, BORDER_WEIGHT, BORDER_COLOR);

        if (dtb::Configs::getDebugMode())
        {
            DrawFPS(0, 0);
        }

        renderOutput();

        EndDrawing();
    };
}
