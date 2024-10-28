#include "MovementComponent.h"

#include "Event.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

void MovementComponent::trigger()
{
    if (!isTriggered_ && !isInProgress_ && !path_.empty())
    {
        triggerPath();
    }
}

void MovementComponent::trigger(Vector2I const& direction)
{
    direction_ = direction;

    currentVelocity_ = Vector2Scale(direction, (speed_ * TileData::TILE_SIZE));

    isTriggered_ = true;
}

void MovementComponent::trigger(std::vector<Vector2I> const& path)
{
    if (path.empty())
    {
        return;
    }

    path_ = path;

    triggerPath();
}

void MovementComponent::triggerPath()
{
    Vector2I movementDirection{
        Vector2Subtract(
            path_.rbegin()[1],
            path_.rbegin()[0])};

    trigger(movementDirection);

    //* Remove tilePosition moved from
    path_.pop_back();

    //* Clear path guided movement after last trigger if target reched (path_.size() == 1 (target))
    if (path_.size() <= 1)
    {
        path_.clear();
    }
}

void MovementComponent::activateTrigger()
{
    isTriggered_ = false;
    setInProgress();
}

void MovementComponent::setInProgress()
{
    //* Retrigger movement
    isInProgress_ = true;

    snx::PublisherStatic::publish(Event::ACTION_IN_PROGRESS);
}

void MovementComponent::stopMovement()
{
    isInProgress_ = false;
    currentVelocity_ = Vector2{0, 0};
}

void MovementComponent::clearPath()
{
    isTriggered_ = false;
    path_.clear();
}

Vector2 MovementComponent::distance() const
{
    return Vector2Scale(
        currentVelocity_,
        GetFrameTime());
}

float MovementComponent::length() const
{
    return Vector2Length(distance());
}

Vector2I const& MovementComponent::direction() const
{
    return direction_;
}

void MovementComponent::setSpeed(int speed)
{
    speed_ = speed;
}

float MovementComponent::cumulativeDistanceMoved() const
{
    return cumulativeDistanceMoved_;
}

void MovementComponent::updateCumulativeDistanceMoved()
{
    cumulativeDistanceMoved_ += Vector2Length(distance());
}

void MovementComponent::resetCumulativeDistanceMoved()
{
    cumulativeDistanceMoved_ = 0;
}

bool MovementComponent::isTriggered() const
{
    return isTriggered_;
}

bool MovementComponent::isInProgress() const
{
    return isInProgress_;
}