#ifndef IG20231201203725
#define IG20231201203725

#include <raylib.h>

namespace snx
{
    class IScene
    {
    public:
        virtual void initialize() = 0;
        virtual void update() = 0;
        virtual void deinitialize() = 0;

        virtual ~IScene() = default;

    protected:
        virtual void processInput() = 0;
        virtual void updateState() = 0;
        virtual void renderOutput() = 0;
        virtual void postOutput() = 0;
    };
    //=====================================
}

#endif
