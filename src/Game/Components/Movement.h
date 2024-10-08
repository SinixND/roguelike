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

    // Speed unit is tiles per second
    int speed_{};
    float cumulativeDistanceMoved_{};

    bool isTriggered_{false};
    bool isInProgress_{false};

public:
    Movement() = default;

    explicit Movement(int speed)
        : speed_(speed)
    {
    }

    Vector2I const& direction() const;
    void setSpeed(int speed);

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
    // - Resets inProgress state if moved for one tile
    // - Resets currentVelocity
    // Returns if tilePosition changed
    bool update(
        Position& position,
        Energy& energy,
        Map const& map,
        Position const& heroPosition);

private:
    void setInProgress();
    void stopMovement();

    // Resets trigger, consumes energy and sets InProgress
    void activateTrigger(Energy& energy);

    // Triggers from and adjusts path
    void triggerPath();
};

#endif
