#ifndef IG20240414161039
#define IG20240414161039

#include "raylibEx.h"
#include <array>
#include <raylib.h>

namespace Directions
{
    //* X is right-positive, Y is down-positive
    Vector2 constexpr null{0, 0};
    Vector2I constexpr noDir{0, 0};
    Vector2I constexpr right{1, 0};
    Vector2I constexpr down{0, 1};
    Vector2I constexpr left{-1, 0};
    Vector2I constexpr up{0, -1};

    std::array<Vector2I, 4> const directions{
        Directions::left,
        Directions::right,
        Directions::up,
        Directions::down};

}

#endif
