#include "Movement.h"

#include "Debugger.h"
#include "DeveloperMode.h"
#include "Energy.h"
#include "Event.h"
#include "Position.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

void Movement::trigger(Vector2I const& direction, bool continuous)
{
    direction_ = direction;
    isContinuous_ = continuous;

    currentVelocity_ = Vector2Scale(direction, (speed_ * TileData::TILE_SIZE));

    isTriggered_ = true;

    // snx::PublisherStatic::publish(Event::actionInProgress);

    if (DeveloperMode::isActive())
    {
        snx::debug("Movement triggered\n");
    }
}

void Movement::setInProgress()
{
    // Retrigger movement
    isInProgress_ = true;

    snx::PublisherStatic::publish(Event::actionInProgress);

    if (DeveloperMode::isActive())
    {
        snx::debug("Movement in progress\n");
    }
}
void Movement::stopMovement()
{
    isInProgress_ = false;
    isContinuous_ = false;
    currentVelocity_ = Vector2{0, 0};

    if (DeveloperMode::isActive())
    {
        snx::debug("Movement stopped\n");
    }
}

void Movement::update(Position& heroPosition, Energy& heroEnergy)
{
    // Start movement
    if (isTriggered_)
    {
        isTriggered_ = false;
        heroEnergy.consume(50);
        setInProgress();
    }

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
        heroPosition.move(distance);
        return;
    }

    // Move by remaining distance until TILE_SIZE
    heroPosition.move(
            Vector2ClampValue(
                distance,
                0,
                TileData::TILE_SIZE - (cumulativeDistanceMoved_ - length)));

    // === Moved one tile ===

    // Reset cumulativeDistanceMoved
    cumulativeDistanceMoved_ = 0;

    snx::PublisherStatic::publish(Event::actionFinished);

    // ReTrigger continuous movment
    if (isContinuous_)
    {
        isTriggered_ = true;
        return;
    }

    stopMovement();
}
