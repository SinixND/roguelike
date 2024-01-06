#include "Scene.h"
#include "raylib.h"

namespace snd
{
    void Scene::update()
    {
        processInput();
        updateState();

        BeginDrawing();

        CONSTANTS* constants{CONSTANTS::getInstance()};
        CONFIGS* configs{CONFIGS::getInstance()};

        ClearBackground(constants->getBackgroundColor());

        DrawRectangleLinesEx(Rectangle{0, 0, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())}, BORDER_WEIGHT, BORDER_COLOR);

        if (configs->getDebugMode() == true)
        {
            DrawFPS(0, 0);
        }

        renderOutput();

        EndDrawing();
    };
}