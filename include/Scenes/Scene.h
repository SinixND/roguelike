#ifndef SCENE_H_20231201203725
#define SCENE_H_20231201203725

#include "CONFIGS.h"
#include "CONSTANTS.h"
#include "raylib.h"
#include <iostream>

namespace snd
{
    constexpr float BORDER_WEIGHT{1};
    constexpr Color BORDER_COLOR{GRAY};

    class Scene
    {
    public:
        virtual void update();

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