#include "TransformComponent.h"
#include "Debugger.h"
#include "EventId.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

void triggerByDirection(
    Vector2I* direction_,
    Vector2* velocity_,
    bool* isTriggered_,
    float const& speed_,
    Vector2I const& direction)
{
    *direction_ = direction;

    *velocity_ = Vector2Scale(direction, (speed_ * TileData::tileSize));

    *isTriggered_ = true;
}

//* Triggers from path and adjusts it
void triggerPath(
    Vector2I* direction_,
    Vector2* velocity_,
    bool* isTriggered_,
    float const& speed_,
    std::vector<Vector2I>* path_)
{
    Vector2I movementDirection{
        Vector2Subtract(
            path_->rbegin()[1],
            path_->rbegin()[0])};

    triggerByDirection(
        direction_,
        velocity_,
        isTriggered_,
        speed_,
        movementDirection);

    //* Remove tilePosition moved from
    path_->pop_back();

    //* Clear path guided movement after last trigger if target reched (path_.size() == 1 (target))
    if (path_->size() <= 1)
    {
        path_->clear();
    }
}

void triggerAuto(
    Vector2I* direction_,
    Vector2* velocity_,
    bool* isTriggered_,
    float const& speed_,
    std::vector<Vector2I>* path_,
    bool isInProgress_)
{
    if (!*isTriggered_ && !isInProgress_ && !path_->empty())
    {
        triggerPath(
            direction_,
            velocity_,
            isTriggered_,
            speed_,
            path_);
    }
}

//* Sets direction, velocity and isTriggered
void triggerFromTo(
    Vector2I* direction_,
    Vector2* velocity_,
    bool* isTriggered_,
    float const& speed_,
    Vector2I const& from,
    Vector2I const& to)
{
    triggerByDirection(
        direction_,
        velocity_,
        isTriggered_,
        speed_,
        Vector2Subtract(
            to,
            from));
}

void triggerByPath(
    Vector2I* direction_,
    Vector2* velocity_,
    bool* isTriggered_,
    float const& speed_,
    std::vector<Vector2I>* path_,
    std::vector<Vector2I> const& path)
{
    if (path.empty())
    {
        return;
    }

    *path_ = path;

    triggerPath(
        direction_,
        velocity_,
        isTriggered_,
        speed_,
        path_);
}

void setInProgress(bool* isInProgress_)
{
    //* Retrigger movement
    *isInProgress_ = true;

    snx::PublisherStatic::publish(EventId::ACTION_IN_PROGRESS);
}

//* Resets trigger, consumes energy and sets InProgress
void activateTrigger(
    bool* isTriggered_,
    bool* isInProgress_)
{
    *isTriggered_ = false;
    setInProgress(isInProgress_);
}

void stopMovement(
    Vector2* velocity_,
    bool* isInProgress_)
{
    *isInProgress_ = false;
    *velocity_ = Vector2{0, 0};
}

//* Unsets isTriggered_ and clears path_
void clearPath(
    bool* isTriggered_,
    std::vector<Vector2I>* path_)
{
    *isTriggered_ = false;
    path_->clear();
}

void updateCumulativeDistanceMoved(
    float* cumulativeDistanceMoved_,
    Vector2* velocity_)
{
    *cumulativeDistanceMoved_ += Vector2Length(Vector2Scale(
        *velocity_,
        GetFrameTime()));
}
