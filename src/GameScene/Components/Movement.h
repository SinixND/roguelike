#ifndef IG20240531201830
#define IG20240531201830

#include "Energy.h"
#include "Position.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

class Movement
{
    std::vector<Vector2I> path_{};
    Vector2I direction_{};
    Vector2 currentVelocity_{};

    // Speed unit is tiles per second
    float speed_{};
    float cumulativeDistanceMoved_{};

    bool isTriggered_{false};
    // bool isContinuous_{false};
    bool isInProgress_{false};

private:
    void setInProgress();
    void stopMovement();

    // Resets trigger, consumes energy and sets InProgress
    void processTrigger(Energy& heroEnergy);

    // Triggers from and adjusts path
    void processPath();

public:
    Movement() = default;

    Movement(float speed) 
        : speed_(speed)
    {
    }

    Vector2I const& direction() const;
    void setSpeed(float speed);

    bool isTriggered() const;

    // Unsets isTriggered_ and clears path_
    void abortMovement();

    // Sets direction, currentVelocity and isTriggered_
    void trigger(Vector2I const& direction);
    void trigger(std::vector<Vector2I> const& path);

    // Moves hero with following steps
    // - Starts movment if trigger set OR trigger not set but path available
    // - Consumes energy,
    // - Sets inProgress state,
    // - Skips if action is in progress,
    // - Moves for one tile max
    // - Resets inProgress state if move for one tile
    // - Resets currentVelocity
    void update(Position& heroPosition, Energy& heroEnergy);
};

#endif
