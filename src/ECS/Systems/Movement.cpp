#include "Movement.h"

#include <raylib.h>

namespace snd
{
    Vector2 Movement::left() { return Vector2{-1, 0}; };
    Vector2 Movement::right() { return Vector2{1, 0}; };
    Vector2 Movement::up() { return Vector2{0, -1}; };
    Vector2 Movement::down() { return Vector2{0, 1}; };
}