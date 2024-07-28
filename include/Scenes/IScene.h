#ifndef IG20231201203725
#define IG20231201203725

#include "Colors.h"
#include "raylibEx.h"
#include <raylib.h>

namespace snx
{
    struct IScene
    {
        const float BORDER_WIDTH{1};

        virtual void update() {};

        void drawWindowBorder()
        {
            DrawRectangleLinesEx(
                GetWindowRec(),
                BORDER_WIDTH,
                BORDER_COLOR);
        };

        virtual ~IScene() = default;
    };
}

#endif
