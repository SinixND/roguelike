#ifndef IG20240218231113
#define IG20240218231113

#include "Pathfinder.h"
#include "raylibEx.h"
#include <cstddef>
#include <raylib.h>
#include <raymath.h>

class Movement
{
public:
    size_t range{};
    float speed{}; // pixel per second

public:
    Vector2i& target() { return target_; };
    void setTarget(const Vector2i& target)
    {
        target_ = target;
    };

    bool isTargetSet() { return isTargetSet_; };
    void setIsTargetSet(bool status) { isTargetSet_ = status; };

    bool isMoving() { return isMoving_; };

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
    Vector2i target_{};
    bool isTargetSet_{false};
    bool isMoving_{false};
    Path path_{};
};

#endif