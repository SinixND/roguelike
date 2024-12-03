#ifndef IG20240531201830
#define IG20240531201830

#include "raylibEx.h"
#include <raylib.h>
#include <vector>

//* Movementspeed in tiles per second
constexpr int MOVEMENT_SPEED{20};

class TransformComponent
{
    Vector2I direction_{};
    Vector2 velocity_{};

    int speed_{MOVEMENT_SPEED};
    float cumulativeDistanceMoved_{};

    bool isTriggered_{false};
    bool isInProgress_{false};

public:
    std::vector<Vector2I> path_{};

public:
    //* Unsets isTriggered_ and clears path_
    void clearPath();

    //* Sets direction, currentVelocity and isTriggered_
    void trigger(Vector2I const& from, Vector2I const& to);
    void trigger(Vector2I const& direction);
    void trigger();

    //* Resets trigger, consumes energy and sets InProgress
    void activateTrigger();

    void stopMovement();

    //* Calculate distance for this frame
    Vector2 distance() const;
    float length() const;

public:
    Vector2I const& direction() const;

    float cumulativeDistanceMoved() const;
    void updateCumulativeDistanceMoved();
    void resetCumulativeDistanceMoved();

    bool isTriggered() const;
    bool isInProgress() const;

    //* Triggers from and adjusts path
    void triggerFromPath();

private:
    void setInProgress();
};

void trigger(
    TransformComponent* transformComponent,
    std::vector<Vector2I> const& path);

#endif
