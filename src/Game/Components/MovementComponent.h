#ifndef IG20240531201830
#define IG20240531201830

#include "raylibEx.h"
#include <raylib.h>
#include <vector>

class MovementComponent
{
    std::vector<Vector2I> path_{};
    Vector2I direction_{};
    Vector2 currentVelocity_{};

    //* Speed unit is tiles per second
    int speed_{};
    float cumulativeDistanceMoved_{};

public:
    bool isTriggered{false};
    bool isInProgress{false};

public:
    MovementComponent() = default;

    explicit MovementComponent(int speed)
        : speed_(speed)
    {
    }

    //* Unsets isTriggered_ and clears path_
    void clearPath();

    //* Sets direction, currentVelocity and isTriggered_
    void trigger(Vector2I const& direction);
    void trigger(std::vector<Vector2I> const& path);
    void trigger();

    //* Resets trigger, consumes energy and sets InProgress
    void activateTrigger();

    void stopMovement();

    //* Calculate distance for this frame
    Vector2 distance() const;
    float length() const;

    Vector2I const& direction() const;
    void setSpeed(int speed);

    float cumulativeDistanceMoved() const;
    void updateCumulativeDistanceMoved();
    void resetCumulativeDistanceMoved();

private:
    void setInProgress();

    //* Triggers from and adjusts path
    void triggerPath();
};

#endif
