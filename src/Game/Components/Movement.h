#ifndef _20240218231113
#define _20240218231113

#include "Constants.h"
#include "Pathfinder.h"
#include "TilePosition.h"
#include "TileTransformation.h"
#include <cstddef>
#include <iostream>
#include <raymath.h>

class Movement
{
public:
    size_t range{};
    float speed{}; // pixel per second

public:
    TilePosition& target() { return target_; };
    void setTarget(const TilePosition& target)
    {
        target_ = target;
    };

    bool isTargetSet() { return targetSet_; };
    void setIsTargetSet(bool status) { targetSet_ = status; };

    bool isMoving() { return isMoving_; };
    //* void setIsMoving(bool state) { isMoving_ = state; };

    bool isExhausted() { return isExhausted_; };
    void setIsExhausted(bool state) { isExhausted_ = state; };

    void setPath(const Path& path)
    {
        path_ = path;
        isMoving_ = true;
    };

    // Returns if movement finished
    bool move(Vector2& position, float dt);

    Movement(
        size_t range,
        float speed)
        : range(range)
        , speed(speed){};

private:
    TilePosition target_{};
    bool targetSet_{false};
    bool isMoving_{false};
    bool isExhausted_{false};
    Path path_{};
};

#endif