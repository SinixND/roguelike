#include "MovementSystem.h"
#include "CollisionSystem.h"
#include "Enemies.h"
#include "EnergyComponent.h"
#include "EventId.h"
#include "MovementComponent.h"
#include "PositionComponent.h"
#include "PublisherStatic.h"
#include "TileData.h"
#include "Tiles.h"
#include "TransformComponent.h"
#include "raylibEx.h"
#include <raymath.h>

void MovementSystem::update(
    TransformComponent& transform,
    MovementComponent& movement,
    PositionComponent& position,
    EnergyComponent& energy,
    PositionComponent const& heroPosition)
{
    //* Start movement from path
    if (!movement.path.empty()
        && !transform.speed)
    {
        prepareFromExistingPath(
            movement,
            transform);
    }

    //* Check if action is in progress
    if (transform.speed)
    {
        energy.consume();

        snx::PublisherStatic::publish(EventId::MULTIFRAME_ACTION_ACTIVE);
    }
    else
    {
        return;
    }

    Vector2 offset{frameOffset(transform)};

    transform.cumulativeDistance += Vector2Length(offset);

    Vector2I oldPosition{position.tilePosition()};

    //* Check if movement exceeds tile length this frame
    if (transform.cumulativeDistance < TileData::tileSize)
    {
        //* Move full distance this frame
        position.move(offset);
    }
    else
    {
        //* Move by remaining distance until TILE_SIZE
        position.move(
            Vector2ClampValue(
                offset,
                0,
                TileData::tileSize
                    - (transform.cumulativeDistance
                       - Vector2Length(offset))));

        //* === Moved one tile ===
        //* Clean precision errors
        position.worldPixel = Vector2Round(position.worldPixel);

        //* Reset cumulativeDistance
        resetCumulativeDistance(transform);

        resetTransform(transform);

        snx::PublisherStatic::publish(EventId::MULTIFRAME_ACTION_DONE);
    }

    //* Check if unit moving is the hero
    if (position.worldPixel != heroPosition.worldPixel)
    {
        return;
    }

    //* Handle special case for hero
    snx::PublisherStatic::publish(EventId::HERO_MOVED);

    if (oldPosition != position.tilePosition())
    {
        snx::PublisherStatic::publish(EventId::HERO_POSITION_CHANGED);
    }
}

void MovementSystem::updateEnemies(
    Enemies& enemies,
    PositionComponent const& heroPosition)
{
    PositionComponent* currentPosition{};
    Vector2I oldPosition{};

    for (size_t i{0}; i < enemies.transforms.size(); ++i)
    {
        currentPosition = &enemies.positions.values().at(i);

        oldPosition = currentPosition->tilePosition();

        //* Update movement
        //* Update ids_ key if tilePosition changes
        MovementSystem::update(
            enemies.transforms.values().at(i),
            enemies.movements.values().at(i),
            *currentPosition,
            enemies.energies.values().at(i),
            heroPosition);

        if (oldPosition != currentPosition->tilePosition())
        {
            enemies.ids.changeKey(
                oldPosition,
                currentPosition->tilePosition());
        }
    }
}

void MovementSystem::prepareByDirection(
    MovementComponent const& movement,
    Vector2I const& direction,
    TransformComponent& transform)
{
    transform.direction = direction;
    transform.speed = movement.baseSpeed;
}

void MovementSystem::prepareFromExistingPath(
    MovementComponent& movement,
    TransformComponent& transform)
{
    prepareByDirection(
        movement,
        Vector2Subtract(
            movement.path.rbegin()[1],
            movement.path.rbegin()[0]),
        transform);

    //* Remove tilePosition moved from
    movement.path.pop_back();

    //* Clear path guided movement after last trigger if target reched (path_.size() == 1 (target))
    if (movement.path.size() <= 1)
    {
        movement.path.clear();
    }
}

void MovementSystem::prepareByNewPath(
    MovementComponent& movement,
    std::vector<Vector2I> const& path)
{
    if (path.empty())
    {
        return;
    }

    movement.path = path;
}

void MovementSystem::prepareByFromTo(
    MovementComponent& movement,
    TransformComponent& transform,
    Vector2I const& from,
    Vector2I const& to)
{
    prepareByDirection(
        movement,
        Vector2Subtract(
            to,
            from),
        transform);
}

void MovementSystem::resetTransform(TransformComponent& transform)
{
    // transform.isInProgress_ = false;
    transform.direction = Vector2I{0, 0};
    transform.speed = .0f;
}

void MovementSystem::resetCumulativeDistance(TransformComponent& transform)
{
    transform.cumulativeDistance = 0;
}

Vector2 MovementSystem::frameOffset(TransformComponent const& transform)
{
    return Vector2Scale(
        transform.direction,
        transform.speed * GetFrameTime());
}
