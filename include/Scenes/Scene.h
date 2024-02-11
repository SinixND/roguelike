#ifndef _20231201203725
#define _20231201203725

#include <raylib.h>

namespace snd
{
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