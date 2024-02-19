#ifndef _20240218231113
#define _20240218231113

#include "raylibEx.h"
#include <cstddef>

class Movement
{
public:
    size_t range() { return range_; };
    void setRange(size_t range) { range_ = range; };

    Vector2Int& target() { return target_; };
    void setTarget(const Vector2Int& target) { target_ = target; };

    float speed() { return speed_; };
    void setSpeed(float speed) { speed_ = speed; };

private:
    size_t range_{};
    Vector2Int target_{};
    float speed_{};
};

#endif