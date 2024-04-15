#include "InputMode.h"

#include <raylib.h>
namespace
{
    static bool isMouseControlled_{true};
}

namespace InputMode
{
    void update()
    {
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            isMouseControlled_ = !isMouseControlled_;

            if (isMouseControlled_)
            {
                ShowCursor();
            }
            else
            {
                HideCursor();
            }
        }
    }

    auto isMouseControlled() -> bool&
    {
        return isMouseControlled_;
    }
}
