#ifndef _20231201203725
#define _20231201203725

#include <raylib.h>

namespace snd
{
    class Scene
    {
    public:
        virtual void update();

        virtual void initialize() = 0;
        virtual void deinitialize() = 0;

        virtual ~Scene() = default;

    protected:
        virtual void processInput() = 0;
        virtual void updateState() = 0;
        virtual void renderOutput() = 0;
        virtual void postOutput() = 0;
    };
    //=====================================
}

#endif