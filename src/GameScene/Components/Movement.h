#ifndef IG20240531201830
#define IG20240531201830

#include "Position.h"
#include "raylibEx.h"
#include <raylib.h>

class Movement
{
    Vector2 currentVelocity_{};
    // Speed unit is tiles per second
    float speed_{};
    float cumulativeDistanceMoved_{};

public:
    void setSpeed(float speed) { speed_ = speed; }

    void trigger(Vector2I const& direction);
    // Moves position by one tile in direction provided via currentVelocity
    void update(Position& position);
};

#endif