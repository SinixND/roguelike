#include "Scene.h"

#include "Globals.h"
#include <raylib.h>

constexpr float BORDER_WEIGHT{1};
constexpr Color BORDER_COLOR{GRAY};

namespace snd
{
    Globals* globals{Globals::getInstance()};

    Scene::~Scene() {}

    void Scene::initialize() {}

    void Scene::update()
    {
        processInput();
        updateState();

        BeginDrawing();
        ClearBackground(globals->getBackgroundColor());

        DrawRectangleLinesEx(Rectangle{0, 0, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())}, BORDER_WEIGHT, BORDER_COLOR);

        if (globals->getDebugMode() == true)
        {
            DrawFPS(0, 0);
        }

        renderOutput();

        EndDrawing();
    }

    void Scene::deinitialize() {}

    void Scene::processInput() {}
    void Scene::updateState() {}
    void Scene::renderOutput() {}

}