#ifndef SCENE_H_20231201203725
#define SCENE_H_20231201203725

#include "Globals.h"
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

            Globals* globals{Globals::getInstance()};

            ClearBackground(globals->getBackgroundColor());

            DrawRectangleLinesEx(Rectangle{0, 0, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())}, BORDER_WEIGHT, BORDER_COLOR);

            if (globals->getDebugMode() == true)
            {
                DrawFPS(0, 0);
            }

            renderOutput();

            EndDrawing();
        };

        virtual ~Scene(){};
        virtual void initialize(){};
        virtual void deinitialize(){};

    protected:
        virtual void processInput(){};
        virtual void updateState(){};
        virtual void renderOutput(){};
    };
    //-------------------------------------
}

#endif