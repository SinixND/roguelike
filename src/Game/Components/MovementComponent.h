#ifndef IG20241208221527
#define IG20241208221527

#include "raylibEx.h"
#include <vector>

//* Movementspeed in tiles per second
constexpr int MOVEMENT_SPEED{10};

struct MovementComponent
{
    std::vector<Vector2I> path_{};
    int speed_{MOVEMENT_SPEED};
};

#endif