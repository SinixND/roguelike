#ifndef IG20240531201830
#define IG20240531201830

#include "raylibEx.h"
#include <raylib.h>
#include <vector>

struct TransformComponent
{
    std::vector<Vector2I> path_{};
    Vector2I direction_{};
    Vector2 velocity_{};

    //* Speed unit is tiles per second
    float speed_{};
    float cumulativeDistanceMoved_{};

    bool isTriggered_{false};
    bool isInProgress_{false};

public:
    TransformComponent() = default;

    explicit TransformComponent(float speed)
        : speed_(speed)
    {
    }
};

void triggerByDirection(
    Vector2I* direction_,
    Vector2* velocity_,
    bool* isTriggered_,
    float const& speed_,
    Vector2I const& direction);

void triggerAuto(
    Vector2I* direction_,
    Vector2* velocity_,
    bool* isTriggered_,
    float const& speed_,
    std::vector<Vector2I>* path_,
    bool isInProgress_);

//* Sets direction, velocity and isTriggered
void triggerFromTo(
    Vector2I* direction_,
    Vector2* velocity_,
    bool* isTriggered_,
    float const& speed_,
    Vector2I const& from,
    Vector2I const& to);

void triggerByPath(
    Vector2I* direction_,
    Vector2* velocity_,
    bool* isTriggered_,
    float const& speed_,
    std::vector<Vector2I>* path_,
    std::vector<Vector2I> const& path);

//* Resets trigger, consumes energy and sets InProgress
void activateTrigger(
    bool* isTriggered_,
    bool* isInProgress_);

void stopMovement(
    Vector2* velocity_,
    bool* isInProgress_);

//* Unsets isTriggered_ and clears path_
void clearPath(
    bool* isTriggered_,
    std::vector<Vector2I>* path_);

void updateCumulativeDistanceMoved(
    float* cumulativeDistanceMoved_,
    Vector2* velocity_);

#endif
