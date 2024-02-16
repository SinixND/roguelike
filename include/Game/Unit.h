#ifndef _20240211230403
#define _20240211230403

#include "GameObject.h"
#include <cstddef>

class Unit : public GameObject
{
public:
    size_t moveRange() { return moveRange_; };
    void setMoveRange(size_t moveRange) { moveRange_ = moveRange; };

private:
    size_t moveRange_{};
};

#endif