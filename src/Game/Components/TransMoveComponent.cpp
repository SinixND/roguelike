#include "TransMoveComponent.h"

#include "EventId.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

void TransMoveComponent::trigger()
{
    if (!isTriggered_ && !isInProgress_ && !path_.empty())
    {
        triggerFromPath();
    }
}

void TransMoveComponent::trigger(Vector2I const& from, Vector2I const& to)
{
    trigger(
        Vector2Subtract(
            to,
            from));
}

void TransMoveComponent::trigger(Vector2I const& direction)
{
    direction_ = direction;

    speed_ = (tilesPerSecond * TileData::tileSize);

    isTriggered_ = true;
}

void trigger(
    TransMoveComponent* transformComponent,
    std::vector<Vector2I> const& path)
{
    if (path.empty())
    {
        return;
    }

    transformComponent->path_ = path;

    transformComponent->triggerFromPath();
}

void TransMoveComponent::triggerFromPath()
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

void TransMoveComponent::activateTrigger()
{
    isTriggered_ = false;
    setInProgress();
}

void TransMoveComponent::setInProgress()
{
    //* Retrigger movement
    isInProgress_ = true;

    snx::PublisherStatic::publish(EventId::ACTION_IN_PROGRESS);
}

void TransMoveComponent::stopMovement()
{
    isInProgress_ = false;
    speed_ = 0;
}

void TransMoveComponent::clearPath()
{
    isTriggered_ = false;
    path_.clear();
}

Vector2 TransMoveComponent::distance() const
{
    return Vector2Scale(
        direction_,
        speed_ * GetFrameTime());
}

float TransMoveComponent::length() const
{
    return Vector2Length(distance());
}

Vector2I const& TransMoveComponent::direction() const
{
    return direction_;
}

float TransMoveComponent::cumulativeDistanceMoved() const
{
    return cumulativeDistanceMoved_;
}

void TransMoveComponent::updateCumulativeDistanceMoved()
{
    cumulativeDistanceMoved_ += Vector2Length(distance());
}

void TransMoveComponent::resetCumulativeDistanceMoved()
{
    cumulativeDistanceMoved_ = 0;
}

bool TransMoveComponent::isTriggered() const
{
    return isTriggered_;
}

bool TransMoveComponent::isInProgress() const
{
    return isInProgress_;
}
