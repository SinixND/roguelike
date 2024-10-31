#include "MovementSystem.h"
#include "CollisionSystem.h"
#include "EnergyComponent.h"
#include "Event.h"
#include "Map.h"
#include "MovementComponent.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "raylibEx.h"
#include <raymath.h>

bool MovementSystem::update(
    MovementComponent& movement,
    PositionComponent& position,
    EnergyComponent& energy,
    Map const& map,
    PositionComponent const& heroPosition)
{
    //* Avoid check if no movement in progress
    if (movement.isTriggered())
    {
        //* Check collision before starting movement
        if (CollisionSystem::checkCollision(
                map,
                Vector2Add(
                    position.tilePosition(),
                    movement.direction()),
                heroPosition.tilePosition()))
        {
            movement.clearPath();

            //* Wait instead
            energy.consume();
        }
    }

    //* Start movement on trigger
    if (movement.isTriggered())
    {
        movement.activateTrigger();
        energy.consume();
    }

    //* Check if action is in progress
    if (!movement.isInProgress())
    {
        return false;
    }

    bool didTilePositionChange{false};

    movement.updateCumulativeDistanceMoved();

    //* Check if movement exceeds tile length this frame
    if (movement.cumulativeDistanceMoved() < TileData::TILE_SIZE)
    {
        //* Move full distance this frame
        didTilePositionChange = position.move(movement.distance());
    }
    else
    {
        //* Move by remaining distance until TILE_SIZE
        didTilePositionChange = position.move(
            Vector2ClampValue(
                movement.distance(),
                0,
                TileData::TILE_SIZE - (movement.cumulativeDistanceMoved() - movement.length())));

        //* === Moved one tile ===
        //* Clean precision errors
        position.changeTo(Vector2Round(position.worldPixel()));

        //* Reset cumulativeDistanceMoved
        movement.resetCumulativeDistanceMoved();

        snx::PublisherStatic::publish(Event::ACTION_FINISHED);

        movement.stopMovement();
    }

    //* Check if unit moving is the hero
    if (position.worldPixel() != heroPosition.worldPixel())
    {
        return didTilePositionChange;
    }

    //* Handle special case for hero
    snx::PublisherStatic::publish(Event::HERO_MOVED);

    if (didTilePositionChange)
    {
        snx::PublisherStatic::publish(Event::HERO_POSITION_CHANGED);
    }

    return didTilePositionChange;
}
