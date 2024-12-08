#ifndef IG20240531201830
#define IG20240531201830

#include "raylibEx.h"
#include <raylib.h>

struct TransformComponent
{
    Vector2I direction_{};
    float speed_{};
    float cumulativeDistanceMoved_{};
};

#endif
