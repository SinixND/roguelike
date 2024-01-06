#ifndef SCENE_H_20231201203725
#define SCENE_H_20231201203725

#include "CONFIGS.h"
#include "CONSTANTS.h"
#include <iostream>
#include <raylib.h>

namespace snd
{
    constexpr float BORDER_WEIGHT{1};
    constexpr Color BORDER_COLOR{GRAY};

    class Scene
    {
    public:
        virtual void update()
        {
            processInput();
            updateState();

            BeginDrawing();

            CONSTANTS* constants{CONSTANTS::getInstance()};
            CONFIGS* configs{CONFIGS::getInstance()};

            ClearBackground(constants->getBackgroundColor());

            DrawRectangleLinesEx(Rectangle{0, 0, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())}, BORDER_WEIGHT, BORDER_COLOR);

            std::cout << "RENDERHEIGHT: " << GetRenderHeight() << "\n";
            std::cout << "SCREENHEIGHT: " << GetScreenHeight() << "\n";
            std::cout << "MONITORHEIGHT: " << GetMonitorHeight(GetCurrentMonitor()) << "\n";

            if (configs->getDebugMode() == true)
            {
                DrawFPS(0, 0);
            }

            renderOutput();

            EndDrawing();
        };

        virtual void initialize(){};
        virtual void deinitialize(){};

        virtual ~Scene() = default;

    protected:
        virtual void processInput(){};
        virtual void updateState(){};
        virtual void renderOutput(){};
    };
    //=====================================
}

#endif