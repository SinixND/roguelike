#ifndef SCENE_H_20231201203725
#define SCENE_H_20231201203725

#include <raylib.h>

namespace snd
{
    class Scene
    {
    public:
        virtual void update();

        virtual ~Scene();
        virtual void initialize();
        virtual void deinitialize();

    protected:
        virtual void processInput();
        virtual void updateState();
        virtual void renderOutput();
    };
    //-------------------------------------
}

#endif