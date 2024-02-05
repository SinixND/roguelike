#include "Scene.h"

#include "RuntimeDatabase.h"
#include <raylib.h>
#include <raylibEx.h>

namespace snd
{
    void Scene::update(Camera2D& camera)
    {
        processInput();
        updateState();

        BeginDrawing();

        ClearBackground(dtb::Constants::backgroundColor_);

        if (dtb::Configs::getDebugMode())
        {
            DrawFPS(0, 0);
        }

        BeginMode2D(camera);

        renderOutput(camera);

        EndMode2D();

        // Draw simple frame
        DrawRectangleLinesEx(Rectangle{0, 0, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())}, BORDER_WEIGHT, BORDER_COLOR);

        EndDrawing();
    };
}
