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
    MovementComponent& movement,
    PositionComponent& position,
    EnergyComponent& energy,
    Map const& map,
    PositionComponent const& heroPosition)
{
    //* Avoid check if no movement in progress
    //* Check collision before starting movement
    if (
        transform.speed
        && CollisionSystem::checkCollision(
            map,
            Vector2Add(
                position.tilePosition(),
                transform.direction),
            heroPosition.tilePosition()))
    {
        movement.path.clear();

        //* Wait instead
        energy.consume();
    }

    //* Start movement on trigger
    if (!movement.path.empty())
    {
        transform.direction = Vector2Subtract(
            movement.path.rbegin()[1],
            movement.path.rbegin()[0]);

        transform.speed = movement.baseSpeed;

        energy.consume();

        snx::PublisherStatic::publish(EventId::ACTION_IN_PROGRESS);
    }

    //* Check if action is in progress
    if (!transform.speed)
    {
        return false;
    }

    bool didTilePositionChange{false};

    MovementSystem::updateCumulativeDistance(transform);

    //* Check if movement exceeds tile length this frame
    if (transform.cumulativeDistance < TileData::tileSize)
    {
        //* Move full distance this frame
        didTilePositionChange = position.move(frameOffset(transform));
    }
    else
    {
        //* Move by remaining distance until TILE_SIZE
        didTilePositionChange = position.move(
            Vector2ClampValue(
                frameOffset(transform),
                0,
                TileData::tileSize - (transform.cumulativeDistance - Vector2Length(frameOffset(transform)))));

        //* === Moved one tile ===
        //* Clean precision errors
        position.worldPixel = Vector2Round(position.worldPixel);

        //* Reset cumulativeDistance
        resetCumulativeDistance(transform);

        snx::PublisherStatic::publish(EventId::ACTION_FINISHED);

        resetTransform(transform);
        movement.path.clear();
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

void MovementSystem::prepareFromExistingPath(
    MovementComponent& movement,
    PositionComponent const& position)
{
    prepareByDirection(
        movement,
        position,
        Vector2Subtract(
            movement.path.rbegin()[1],
            movement.path.rbegin()[0]));

    //* Remove tilePosition moved from
    movement.path.pop_back();

    //* Clear path guided movement after last trigger if target reched (path_.size() == 1 (target))
    if (movement.path.size() <= 1)
    {
        movement.path.clear();
    }
}

void MovementSystem::prepareInputAgnostic(
    MovementComponent& movement,
    TransformComponent const& transform,
    PositionComponent const& position)
{
    if (
        !transform.speed
        && !movement.path.empty())
    {
        MovementSystem::prepareFromExistingPath(
            movement,
            position);
    }
}

void MovementSystem::prepareByNewPath(
    MovementComponent& movement,
    PositionComponent const& position,
    std::vector<Vector2I> const& path)
{
    if (path.empty())
    {
        return;
    }

    movement.path = path;

    MovementSystem::prepareFromExistingPath(
        movement,
        position);
}

void MovementSystem::updateCumulativeDistance(TransformComponent& transform)
{
    transform.cumulativeDistance += Vector2Length(frameOffset(transform));
}

void MovementSystem::resetCumulativeDistance(TransformComponent& transform)
{
    transform.cumulativeDistance = 0;
}

void MovementSystem::prepareByDirection(
    MovementComponent& movement,
    PositionComponent const& position,
    Vector2I const& direction)
{
    movement.path.push_back(Vector2Add(
        position.tilePosition(),
        direction));

    movement.path.push_back(position.tilePosition());
}

void MovementSystem::prepareByFromTo(
    MovementComponent& movement,
    PositionComponent const& position,
    Vector2I const& from,
    Vector2I const& to)
{
    prepareByDirection(
        movement,
        position,
        Vector2Subtract(
            to,
            from));
}

void MovementSystem::resetTransform(TransformComponent& transform)
{
    // transform.isInProgress_ = false;
    transform.direction = Vector2I{0, 0};
    transform.speed = .0f;
}

Vector2 MovementSystem::frameOffset(TransformComponent const& transform)
{
    return Vector2Scale(
        transform.direction,
        transform.speed * GetFrameTime());
}
