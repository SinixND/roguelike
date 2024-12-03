#include "MovementSystem.h"
#include "CollisionSystem.h"
#include "EnergyComponent.h"
#include "EventId.h"
#include "Map.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "TransformComponent.h"
#include "raylibEx.h"
#include <raymath.h>

bool MovementSystem::update(
    TransformComponent& transform,
    PositionComponent& position,
    EnergyComponent& energy,
    Map const& map,
    PositionComponent const& heroPosition)
{
    //* Avoid check if no movement in progress
    if (transform.isTriggered())
    {
        //* Check collision before starting movement
        if (CollisionSystem::checkCollision(
                map,
                Vector2Add(
                    position.tilePosition(),
                    transform.direction()),
                heroPosition.tilePosition()))
        {
            transform.clearPath();

            //* Wait instead
            energy.consume();
        }
    }

    //* Start movement on trigger
    if (transform.isTriggered())
    {
        transform.activateTrigger();
        energy.consume();
    }

    //* Check if action is in progress
    if (!transform.isInProgress())
    {
        return false;
    }

    bool didTilePositionChange{false};

    transform.updateCumulativeDistanceMoved();

    //* Check if movement exceeds tile length this frame
    if (transform.cumulativeDistanceMoved() < TileData::tileSize)
    {
        //* Move full distance this frame
        didTilePositionChange = position.move(transform.distance());
    }
    else
    {
        //* Move by remaining distance until TILE_SIZE
        didTilePositionChange = position.move(
            Vector2ClampValue(
                transform.distance(),
                0,
                TileData::tileSize - (transform.cumulativeDistanceMoved() - transform.length())));

        //* === Moved one tile ===
        //* Clean precision errors
        position.worldPixel = Vector2Round(position.worldPixel);

        //* Reset cumulativeDistanceMoved
        transform.resetCumulativeDistanceMoved();

        snx::PublisherStatic::publish(EventId::ACTION_FINISHED);

        transform.stopMovement();
    }

    //* Check if unit moving is the hero
    if (position.worldPixel != heroPosition.worldPixel)
    {
        return didTilePositionChange;
    }

    //* Handle special case for hero
    snx::PublisherStatic::publish(EventId::HERO_MOVED);

    if (didTilePositionChange)
    {
        snx::PublisherStatic::publish(EventId::HERO_POSITION_CHANGED);
    }

    return didTilePositionChange;
}
