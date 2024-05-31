#ifndef IG20231201203725
#define IG20231201203725

#include "raylibEx.h"
#include <raylib.h>

namespace snx
{
    struct IScene
    {
        const float BORDER_WIDTH{1};
        const Color BORDER_COLOR{GRAY};
        const Color BACKGROUND_COLOR{BLACK};

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
