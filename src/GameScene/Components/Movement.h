#ifndef IG20240218231113
#define IG20240218231113

#include "Pathfinder.h"
#include "Position.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

class MovementComponent
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
    void setIsMoving(bool state) { isMoving_ = state; }

    Path const& path() const { return path_; }
    void popFromPath() { path_.pop_back(); }
    void setPath(Path const& path)
    {
        path_ = path;
        isMoving_ = true;
    }

    explicit MovementComponent(int range, float speed)
        : range_(range)
        , speed_(speed)
    {
    }

private:
    Path path_{};
    Vector2I target_{};
    int range_{};
    float speed_{}; // pixel per second

    bool isTargetSet_{false};
    bool isMoving_{false};
};

namespace Movement
{
    // Returns if movement finished
    bool moveAlongPath(PositionComponent& position, MovementComponent& movementComponent, float dt);
}

#endif