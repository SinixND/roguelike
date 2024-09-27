#include "Movement.h"

#include "Collision.h"
#include "Energy.h"
#include "Event.h"
#include "Position.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

Vector2I const& Movement::direction() const
{
    return direction_;
}

void Movement::setSpeed(int agility) { speed_ = agility; }

bool Movement::isTriggered() const
{
    return isTriggered_;
}

void Movement::trigger(Vector2I const& direction)
{
    direction_ = direction;

    currentVelocity_ = Vector2Scale(direction, (speed_ * TileData::TILE_SIZE));

    isTriggered_ = true;
}

void Movement::trigger(std::vector<Vector2I> const& path)
{
    if (path.empty())
    {
        return;
    }

    path_ = path;

    processPath();
}

void Movement::trigger()
{
    if (!isTriggered_ && !isInProgress_ && !path_.empty())
    {
        processPath();
    }
}

void Movement::processPath()
{
    Vector2I direction{
        // Vector2Normalize(
        Vector2Subtract(
            path_.rbegin()[1],
            path_.rbegin()[0])
        // )
    };

    trigger(direction);

    // Remove tilePosition moved from
    path_.pop_back();

    // Clear path guided movement after last trigger if target reched (path_.size() == 1 (target))
    if (path_.size() <= 1)
    {
        path_.clear();
    }
}

void Movement::processTrigger(Energy& energy)
{
    isTriggered_ = false;
    energy.consume(50);
    setInProgress();
}

void Movement::setInProgress()
{
    // Retrigger movement
    isInProgress_ = true;

    snx::PublisherStatic::publish(Event::actionInProgress);
}

void Movement::stopMovement()
{
    isInProgress_ = false;
    currentVelocity_ = Vector2{0, 0};
}

void Movement::abortMovement()
{
    isTriggered_ = false;
    path_.clear();
}

void Movement::update(Position& position, Energy& energy, Map const& map)
{
    // Avoid check if no movement in progress
    if (isTriggered())
    {
        // Check collision before starting movement
        if (Collision::checkCollision(
            map,
            Vector2Add(
                position.tilePosition(),
                direction())))
        {
            abortMovement();
        }
    }

    // Start movement on trigger
    if (isTriggered_)
    {
        processTrigger(energy);
    }

    // Check if action is in progress
    if (!isInProgress_)
    {
        return;
    }

    Vector2 distance{Vector2Scale(currentVelocity_, GetFrameTime())};
    float length{Vector2Length(distance)};

    cumulativeDistanceMoved_ += length;

    // Move for one tile max
    if (cumulativeDistanceMoved_ < TileData::TILE_SIZE)
    {
        position.move(distance);
        snx::PublisherStatic::publish(Event::heroMoved);
        return;
    }

    if (cumulativeDistanceMoved_ > TileData::TILE_SIZE_HALF)
    {
        snx::PublisherStatic::publish(Event::heroPositionChanged);
    }

    // Move by remaining distance until TILE_SIZE
    position.move(
        Vector2ClampValue(
            distance,
            0,
            TileData::TILE_SIZE - (cumulativeDistanceMoved_ - length)));
    snx::PublisherStatic::publish(Event::heroMoved);

    // === Moved one tile ===

    // Reset cumulativeDistanceMoved
    cumulativeDistanceMoved_ = 0;

    snx::PublisherStatic::publish(Event::actionFinished);

    stopMovement();
}
