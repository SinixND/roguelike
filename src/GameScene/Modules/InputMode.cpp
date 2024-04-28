#include "InputMode.h"

#include <raylib.h>

namespace InputMode
{
    static bool isMouseControlled_{true};

    void update()
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

    auto isMouseControlled() -> bool&
    {
        return isMouseControlled_;
    }
}
