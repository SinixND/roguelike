#include "Movement.h"

#include "Position.h"
#include "Textures.h"
#include "TileTransformation.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

void updateTileFraction(float& cumulativeTileFraction, float frameDistance);
void updatePosition(PositionComponent& positionComponent, float frameDistance, Vector2I const& directionAccessed);
void resetTileProgress(float& cumulativeTileFraction);
void adjustPosition(PositionComponent& positionComponent, Vector2I const& targetTilePosition);
void updatePath(MovementComponent& movementComponent);

auto Movement::moveAlongPath(PositionComponent& positionComponent, MovementComponent& movementComponent, float dt) -> bool
{
    // Cumulative fraction of one tile size for smooth path progressing
    static float cumulativeTileFraction{};

    // Distance to move this frame
    float frameDistance = (movementComponent.speed() * Textures::TILE_SIZE) * dt;

    updateTileFraction(
        cumulativeTileFraction,
        frameDistance);

    updatePosition(
        positionComponent,
        frameDistance,
        movementComponent.path().back().directionAccessed);

    // If next tile reached
    if (cumulativeTileFraction > Textures::TILE_SIZE)
    {
        resetTileProgress(cumulativeTileFraction);

        adjustPosition(
            positionComponent,
            movementComponent.path().back().tile->positionComponent.tilePosition());

        updatePath(movementComponent);
    }

    // If target reached
    if (movementComponent.path().empty())
    {
        movementComponent.setIsMoving(false);
        return true;
    }

    return false;
}

void updateTileFraction(float& cumulativeTileFraction, float frameDistance)
{
    cumulativeTileFraction += frameDistance;
}

void updatePosition(PositionComponent& positionComponent, float frameDistance, Vector2I const& directionAccessed)
{
    positionComponent.setRenderPosition(
        Vector2Add(
            positionComponent.renderPosition(),
            Vector2Scale(
                directionAccessed,
                frameDistance)));
}

void resetTileProgress(float& cumulativeTileFraction)
{
    cumulativeTileFraction = 0;
}

void adjustPosition(PositionComponent& positionComponent, Vector2I const& targetTilePosition)
{
    positionComponent.setRenderPosition(
        TileTransformation::positionToWorld(
            targetTilePosition));
}

void updatePath(MovementComponent& movementComponent)
{
    movementComponent.popFromPath();
}