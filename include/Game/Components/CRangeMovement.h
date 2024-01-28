#ifndef CRANGEMOVEMENT_H_20240128153242
#define CRANGEMOVEMENT_H_20240128153242

#include "Component.h"

struct CRangeMovement
    : public snd::Component<CRangeMovement>
{
public:
    int getMovementRange() { return movementRange_; };
    void setMovementRange(int movementRange) { movementRange_ = movementRange; };

    CRangeMovement(int range)
        : movementRange_(range){};

private:
    int movementRange_;
};

#endif