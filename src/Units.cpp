#include "Units.h"
#include <raylib.h>

namespace sxd
{
    Vector2 Unit::left() { return Vector2{-1, 0}; };
    Vector2 Unit::right() { return Vector2{1, 0}; };
    Vector2 Unit::up() { return Vector2{0, -1}; };
    Vector2 Unit::down() { return Vector2{0, 1}; };
    float Unit::field() { return 20; };
}