#ifndef IG20240531201830
#define IG20240531201830

#include "raylibEx.h"
#include <raylib.h>
#include <vector>

class Map;
class PositionComponent;
class EnergyComponent;

class MovementComponent
{
    std::vector<Vector2I> path_{};
    Vector2I direction_{};
    Vector2 currentVelocity_{};

    //* Speed unit is tiles per second
    int speed_{};
    float cumulativeDistanceMoved_{};

    bool isTriggered_{false};
    bool isInProgress_{false};

public:
    MovementComponent() = default;

    explicit MovementComponent(int speed)
        : speed_(speed)
    {
    }

    //* Unsets isTriggered_ and clears path_
    void clearMovment();

    //* Sets direction, currentVelocity and isTriggered_
    void trigger(Vector2I const& direction);
    void trigger(std::vector<Vector2I> const& path);
    void trigger();

    //* Moves hero with following steps
    //* - Check for collision
    //* - Starts movment if trigger set OR trigger not set but path available
    //* - Consumes energy,
    //* - Sets inProgress state,
    //* - Skips if action is in progress,
    //* - Moves for one tile max
    //* - Resets inProgress state if moved for one tile
    //* - Resets currentVelocity
    //* Returns if tilePosition changed
    bool update(
        PositionComponent& position,
        EnergyComponent& energy,
        Map const& map,
        PositionComponent const& heroPosition);

    void setSpeed(int speed);

private:
    void setInProgress();
    void stopMovement();

    //* Resets trigger, consumes energy and sets InProgress
    void activateTrigger(EnergyComponent& energy);

    //* Triggers from and adjusts path
    void triggerPath();
};

#endif
