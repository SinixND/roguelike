#ifndef POSITIONCOMPONENT_H_20231212234424
#define POSITIONCOMPONENT_H_20231212234424

#include <raylib.h>

namespace snd
{
    // typedef Vector2 Position; // has float values...

    struct Position
    {
        int x_;
        int y_;

        Position(int x, int y)
            : x_{x}
            , y_{y} {};
    };
}

#endif