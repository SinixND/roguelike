#ifndef IG20240531201830
#define IG20240531201830

#include "Energy.h"
#include "Position.h"
#include "raylibEx.h"
#include <raylib.h>

class Movement
{
    Vector2I direction_{};
    Vector2 currentVelocity_{};

    // Speed unit is tiles per second
    float speed_{};
    float cumulativeDistanceMoved_{};

    bool isTriggered_{false};
    bool isContinuous_{false};
    bool isInProgress_{false};

private:
    void setInProgress();
    void stopMovement();

public:
    Vector2I const& direction() { return direction_; }
    void setSpeed(float speed) { speed_ = speed; }

    bool isTriggered() const { return isTriggered_; }
    void abortMovement() { 
        isTriggered_ = false; 
    if (DeveloperMode::isActive())
    {
        snx::debug("Movement aborted\n");
    }
    }
    void trigger(
        Vector2I const& direction,
        bool continuous = false);

    // Moves position by one tile in direction provided via currentVelocity
    void update(Position& heroPosition, Energy& heroEnergy);
};

#endif
