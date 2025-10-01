#ifndef IG20240531201830
#define IG20240531201830

//* DEPRECATED

#include "raylibEx.h"
#include <raylib.h>

struct TransformComponent
{
    Vector2I direction{};
    float speed{};
    float remainingDistance{};
};

#endif
