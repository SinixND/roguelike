#ifndef IG20240414161039
#define IG20240414161039

#include "raylibEx.h"
#include <raylib.h>

namespace Directions
{
    // X is right-positive, Y is down-positive
    Vector2 constexpr V_NULL{0, 0};
    Vector2I constexpr V_NODIR{0, 0};
    Vector2I constexpr V_RIGHT{1, 0};
    Vector2I constexpr V_DOWN{0, 1};
    Vector2I constexpr V_LEFT{-1, 0};
    Vector2I constexpr V_UP{0, -1};

    std::array<Vector2I, 4> const directions{
        Directions::V_LEFT,
        Directions::V_RIGHT,
        Directions::V_UP,
        Directions::V_DOWN};

}

#endif
