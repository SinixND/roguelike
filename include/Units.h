#ifndef DIRECTION_H_20231204001446
#define DIRECTION_H_20231204001446

struct Vector2;

namespace snd
{
    struct Unit
    {
        static Vector2 left();
        static Vector2 right();
        static Vector2 up();
        static Vector2 down();
        static float field();
    };
}

#endif