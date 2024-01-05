#ifndef SCENE_H_20231201203725
#define SCENE_H_20231201203725

#include "CONFIGS.h"
#include "CONSTANTS.h"
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

            if (configs->getDebugMode() == true)
            {
                DrawFPS(0, 0);
            }

            renderOutput();

            EndDrawing();
        };

        virtual void initialize(){};
        virtual void deinitialize(){};

        Scene() = default;
        virtual ~Scene() = default;
        Scene(const Scene&) = default;
        Scene& operator=(const Scene&) = default;
        Scene(Scene&&) = default;
        Scene& operator=(Scene&&) = default;

    protected:
        virtual void processInput(){};
        virtual void updateState(){};
        virtual void renderOutput(){};
    };
    //=====================================
}

#endif