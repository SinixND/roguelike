#ifndef IG20240218231113
#define IG20240218231113

#include "Pathfinder.h"
#include "Transformation.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

class Movement
{
public:
    int range() const { return range_; }
    void setRange(int range) { range_ = range; }

    float speed() const { return speed_; }
    void setSpeed(float speed) { speed_ = speed; }

    Vector2I target() const { return target_; }
    void setTarget(Vector2I target)
    {
        target_ = target;
        isTargetSet_ = true;
    }

    bool isTargetSet() const { return isTargetSet_; }
    void unsetTarget() { isTargetSet_ = false; }

    bool isMoving() const { return isMoving_; }

    void setPath(Path const& path)
    {
        path_ = path;
        isMoving_ = true;
    }

    // Returns if movement finished
    bool move(Transformation& transform, float dt);

    Movement(int range, float speed)
        : range_(range)
        , speed_(speed)
    {
    }

private:
    int range_{};
    float speed_{}; // pixel per second
    Vector2I target_{};
    bool isTargetSet_{false};
    bool isMoving_{false};
    Path path_{};
};

#endif