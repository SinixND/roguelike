#include "TransformComponent.h"

#include "EventId.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

void TransformComponent::trigger()
{
    if (!isTriggered_ && !isInProgress_ && !path_.empty())
    {
        triggerFromPath();
    }
}

void TransformComponent::trigger(Vector2I const& from, Vector2I const& to)
{
    trigger(
        Vector2Subtract(
            to,
            from));
}

void TransformComponent::trigger(Vector2I const& direction)
{
    direction_ = direction;

    velocity_ = Vector2Scale(
        direction,
        (speed_ * TileData::tileSize));

    isTriggered_ = true;
}

void trigger(
    TransformComponent* transformComponent,
    std::vector<Vector2I> const& path)
{
    if (path.empty())
    {
        return;
    }

    transformComponent->path_ = path;

    transformComponent->triggerFromPath();
}

void TransformComponent::triggerFromPath()
{
    trigger(Vector2Subtract(
        path_.rbegin()[1],
        path_.rbegin()[0]));

    //* Remove tilePosition moved from
    path_.pop_back();

    //* Clear path guided movement after last trigger if target reched (path_.size() == 1 (target))
    if (path_.size() <= 1)
    {
        path_.clear();
    }
}

void TransformComponent::activateTrigger()
{
    isTriggered_ = false;
    setInProgress();
}

void TransformComponent::setInProgress()
{
    //* Retrigger movement
    isInProgress_ = true;

    snx::PublisherStatic::publish(EventId::ACTION_IN_PROGRESS);
}

void TransformComponent::stopMovement()
{
    isInProgress_ = false;
    velocity_ = Vector2{0, 0};
}

void TransformComponent::clearPath()
{
    isTriggered_ = false;
    path_.clear();
}

Vector2 TransformComponent::distance() const
{
    return Vector2Scale(
        velocity_,
        GetFrameTime());
}

float TransformComponent::length() const
{
    return Vector2Length(distance());
}

Vector2I const& TransformComponent::direction() const
{
    return direction_;
}

float TransformComponent::cumulativeDistanceMoved() const
{
    return cumulativeDistanceMoved_;
}

void TransformComponent::updateCumulativeDistanceMoved()
{
    cumulativeDistanceMoved_ += Vector2Length(distance());
}

void TransformComponent::resetCumulativeDistanceMoved()
{
    cumulativeDistanceMoved_ = 0;
}

bool TransformComponent::isTriggered() const
{
    return isTriggered_;
}

bool TransformComponent::isInProgress() const
{
    return isInProgress_;
}
