#include "MovementComponent.h"

#include "CollisionSystem.h"
#include "EnergyComponent.h"
#include "Event.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "raylibEx.h"
#include <cmath>
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

void MovementComponent::activateTrigger(EnergyComponent& energy)
{
    isTriggered_ = false;
    energy.consume();
    setInProgress();
}

void MovementComponent::setInProgress()
{
    //* Retrigger movement
    isInProgress_ = true;

    snx::PublisherStatic::publish(Event::actionInProgress);
}

void MovementComponent::stopMovement()
{
    isInProgress_ = false;
    currentVelocity_ = Vector2{0, 0};
}

void MovementComponent::clearMovment()
{
    isTriggered_ = false;
    path_.clear();
}

bool MovementComponent::update(
    PositionComponent& position,
    EnergyComponent& energy,
    Map const& map,
    PositionComponent const& heroPosition)
{
    //* Avoid check if no movement in progress
    if (isTriggered_)
    {
        //* Check collision before starting movement
        if (CollisionSystem::checkCollision(
                map,
                Vector2Add(
                    position.tilePosition(),
                    direction_),
                heroPosition.tilePosition()))
        {
            clearMovment();
            //* Wait instead
            energy.consume();
        }
    }

    //* Start movement on trigger
    if (isTriggered_)
    {
        activateTrigger(energy);
    }

    //* Check if action is in progress
    if (!isInProgress_)
    {
        return false;
    }

    Vector2 distance{Vector2Scale(currentVelocity_, GetFrameTime())};
    float length{Vector2Length(distance)};
    bool didTilePositionChange{false};

    cumulativeDistanceMoved_ += length;

    //* Check if movement exceeds tile length this frame
    if (cumulativeDistanceMoved_ < TileData::TILE_SIZE)
    {
        didTilePositionChange = position.move(distance);
    }
    else
    {
        //* Move by remaining distance until TILE_SIZE
        didTilePositionChange = position.move(
            Vector2ClampValue(
                distance,
                0,
                TileData::TILE_SIZE - (cumulativeDistanceMoved_ - length)));

        //* === Moved one tile ===
        //* Clean precision errors
        position.changeTo(Vector2Round(position.worldPixel()));

        //* Reset cumulativeDistanceMoved
        cumulativeDistanceMoved_ = 0;

        snx::PublisherStatic::publish(Event::actionFinished);

        stopMovement();
    }

    //* Check if unit moving is the hero
    if (position.worldPixel() != heroPosition.worldPixel())
    {
        return didTilePositionChange;
    }

    //* Handle special case for hero
    snx::PublisherStatic::publish(Event::heroMoved);

    if (didTilePositionChange)
    {
        snx::PublisherStatic::publish(Event::heroPositionChanged);
    }

    return didTilePositionChange;
}

void MovementComponent::setSpeed(int speed) { speed_ = speed; }
