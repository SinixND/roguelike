#ifndef IG20231201203725
#define IG20231201203725

#include "Colors.h"
#include "raylibEx.h"
#include <raylib.h>

namespace snx
{
    class IScene
    {
        float const borderWidth_{ 1 };

    public:
        virtual ~IScene() = default;

        virtual void update() {}

        void drawWindowBorder()
        {
            DrawRectangleLinesEx(
                GetWindowRec(),
                borderWidth_,
                Colors::border
            );
        }
    };
}

#endif
