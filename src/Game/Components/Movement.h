#ifndef IG20240531201830
#define IG20240531201830

#include "Energy.h"
#include "Position.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

struct Map;

class Movement
{
    std::vector<Vector2I> path_{};
    Vector2I direction_{};
    Vector2 currentVelocity_{};

    // speed unit is tiles per second
    int speed_{10};
    float cumulativeDistanceMoved_{};

    bool isTriggered_{false};
    // bool isContinuous_{false};
    bool isInProgress_{false};

public:
    Movement() = default;

    Movement(int agility)
        : speed_(10 * agility)
    {
    }

    Vector2I const& direction() const;
    void setSpeed(int agility);

    bool isTriggered() const;

    // Unsets isTriggered_ and clears path_
    void abortMovement();

    // Sets direction, currentVelocity and isTriggered_
    void trigger(Vector2I const& direction);
    void trigger(std::vector<Vector2I> const& path);
    void trigger();

    // Moves hero with following steps
    // - Check for collision
    // - Starts movment if trigger set OR trigger not set but path available
    // - Consumes energy,
    // - Sets inProgress state,
    // - Skips if action is in progress,
    // - Moves for one tile max
    // - Resets inProgress state if move for one tile
    // - Resets currentVelocity
void update(
        Position& position, 
        Energy& energy, 
        Map const& map);

private:
    void setInProgress();
    void stopMovement();

    // Resets trigger, consumes energy and sets InProgress
    void processTrigger(Energy& energy);

    // Triggers from and adjusts path
    void processPath();
};

#endif
