#ifndef IG20240531201830
#define IG20240531201830

#include "raylibEx.h"
#include <raylib.h>
#include <vector>

//* Movementspeed in tiles per second
constexpr int MOVEMENT_SPEED{20};

struct MovementComponent
{
    std::vector<Vector2I> path_{};
    int baseSpeed_{MOVEMENT_SPEED};
    float cumulativeDistanceMoved_{};
};

#endif
