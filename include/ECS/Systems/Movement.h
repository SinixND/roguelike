#ifndef MOVEMENT_H_20231218011113
#define MOVEMENT_H_20231218011113

#include <raylib.h>

namespace snd
{
    class Movement
    {
    public:
        Vector2 left();
        Vector2 right();
        Vector2 up();
        Vector2 down();
    };
}

#endif