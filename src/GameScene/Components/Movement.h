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
    auto range() const -> int { return range_; }
    void setRange(int range) { range_ = range; }

    auto speed() const -> float { return speed_; }
    void setSpeed(float speed) { speed_ = speed; }

    auto target() const -> Vector2I { return target_; }
    void setTarget(Vector2I target)
    {
        target_ = target;
        isTargetSet_ = true;
    }

    auto isTargetSet() const -> bool { return isTargetSet_; }
    void unsetTarget() { isTargetSet_ = false; }

    auto isMoving() const -> bool { return isMoving_; }
    void setIsMoving(bool state) { isMoving_ = state; }

    auto path() const -> Path const& { return path_; }
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
    auto moveAlongPath(PositionComponent& position, MovementComponent& movementComponent, float dt) -> bool;
}

#endif