#ifndef CRANGEMOVEMENT_H_20240128153242
#define CRANGEMOVEMENT_H_20240128153242

#include "Component.h"
#include <cstddef>

struct CRangeMovement
    : public snd::Component<CRangeMovement>
{
public:
    size_t getMovementRange() { return movementRange_; };
    void setMovementRange(size_t movementRange) { movementRange_ = movementRange; };

    CRangeMovement(size_t range)
        : movementRange_(range){};

private:
    size_t movementRange_;
};

#endif