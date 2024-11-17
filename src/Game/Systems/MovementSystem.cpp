#include "MovementSystem.h"

#include "CollisionSystem.h"
#include "Convert.h"
#include "EnergyComponent.h"
#include "EventId.h"
#include "Map.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "TransformComponent.h"
#include "raylibEx.h"
#include <raymath.h>

//* Returns true if tilePosition changed
bool move(
    Vector2* position,
    Vector2 const& offset)
{
    Vector2I oldPosition{Convert::worldToTile(*position)};

    *position += offset;

    if (!Vector2Equals(
            oldPosition,
            Convert::worldToTile(*position)))
    {
        return true;
    }

    return false;
}

bool MovementSystem::update(
    TransformComponent& movement,
    Vector2& position,
    EnergyComponent& energy,
    Map const& map,
    Vector2 const& heroPosition)
{
    //* Avoid check if no movement in progress
    if (movement.isTriggered_)
    {
        //* Check collision before starting movement
        if (CollisionSystem::checkCollision(
                map,
                Vector2Add(
                    Convert::worldToTile(position),
                    movement.direction_),
                Convert::worldToTile(heroPosition)))
        {
            clearPath(
                &movement.isTriggered_,
                &movement.path_);

            //* Wait instead
            consume(&energy);
        }
    }

    //* Start movement on trigger
    if (movement.isTriggered_)
    {
        activateTrigger(
            &movement.isTriggered_,
            &movement.isInProgress_);

        consume(&energy);
    }

    //* Check if action is in progress
    if (!movement.isInProgress_)
    {
        return false;
    }

    bool didTilePositionChange{false};

    updateCumulativeDistanceMoved(
        &movement.cumulativeDistanceMoved_,
        &movement.velocity_);

    //* Check if movement exceeds tile length this frame
    if (movement.cumulativeDistanceMoved_ < TileData::tileSize)
    {
        //* Move full distance this frame
        didTilePositionChange = move(
            &position,
            Vector2Scale(
                movement.velocity_,
                GetFrameTime()));
    }
    else
    {
        //* Move by remaining distance until TILE_SIZE
        didTilePositionChange = move(
            &position,
            Vector2ClampValue(
                Vector2Scale(
                    movement.velocity_,
                    GetFrameTime()),
                0,
                TileData::tileSize
                    - (movement.cumulativeDistanceMoved_ - Vector2Length(Vector2Scale(movement.velocity_, GetFrameTime())))));

        //* === Moved one tile ===
        //* Clean precision errors
        position = Vector2Round(position);

        //* Reset cumulativeDistanceMoved
        movement.cumulativeDistanceMoved_ = 0;

        snx::PublisherStatic::publish(EventId::ACTION_FINISHED);

        stopMovement(
            &movement.velocity_,
            &movement.isInProgress_);
    }

    //* Check if unit moving is the hero
    if (position != heroPosition)
    {
        return didTilePositionChange;
    }

    //* Handle special case for hero
    snx::PublisherStatic::publish(EventId::HERO_MOVED);

    if (didTilePositionChange)
    {
        snx::PublisherStatic::publish(EventId::HERO_POSITION__CHANGED);
    }

    return didTilePositionChange;
}
