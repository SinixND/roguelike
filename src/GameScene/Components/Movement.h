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
    Path const& movepath() { return movepath_; }

    void setMovePath(Path const& movepath) { movepath_ = movepath; }

    void popFromMovepath() { movepath_.pop_back(); }

    size_t range() const { return range_; }

    void setRange(size_t range) { range_ = range; }

    float speed() const { return speed_; }

    void setSpeed(float speed) { speed_ = speed; }

    Vector2i target() const { return target_; }

    void setTarget(Vector2i target) { target_ = target; }

    bool isTargetSet() const { return isTargetSet_; }

    void setIsTargetSet(bool status) { isTargetSet_ = status; }

    bool isMoving() const { return isMoving_; }

    void setIsMoving(bool isMoving) { isMoving_ = isMoving; }

    Movement(size_t range, float speed)
        : range_(range)
        , speed_(speed)
    {}

private:
    Path movepath_{};
    Vector2i target_{};
    size_t range_{};

    float speed_{}; // pixel per second
    bool isTargetSet_{false};
    bool isMoving_{false};
};

#endif