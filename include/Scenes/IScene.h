#ifndef IG20231201203725
#define IG20231201203725

#include "Colors.h"
#include "raylibEx.h"
#include <raylib.h>

namespace snx
{
    struct IScene
    {
        float const borderWidth{ 1 };

        virtual void update() {}

        void drawWindowBorder()
        {
            DrawRectangleLinesEx(
                GetWindowRec(),
                borderWidth,
                Colors::border
            );
        }

        virtual ~IScene() = default;
    };
}

#endif
