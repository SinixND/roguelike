#include "Movement.h"

#include "Position.h"
#include "TextureData.h"
#include "TileTransformation.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

void updatePath(MovementComponent* movementComponent)
{
    movementComponent->popFromPath();
}

void adjustPosition(PositionComponent* positionComponent, Vector2I const& targetTilePosition)
{
    positionComponent->setRenderPosition(
        TileTransformation::positionToWorld(
            targetTilePosition));
}

void resetTileProgress(float& cumulativeTileFraction)
{
    cumulativeTileFraction = 0;
}

void updatePosition(PositionComponent* positionComponent, float frameDistance, Vector2I const& directionAccessed)
{
    positionComponent->setRenderPosition(
        Vector2Add(
            positionComponent->renderPosition(),
            Vector2Scale(
                directionAccessed,
                frameDistance)));
}

void updateTileFraction(float& cumulativeTileFraction, float frameDistance)
{
    cumulativeTileFraction += frameDistance;
}

bool isMovementComplete(MovementComponent* movementComponent)
{
    if (movementComponent->path().empty())
    {
        movementComponent->setIsMoving(false);
        return true;
    }

    return false;
}
bool Movement::moveAlongPath(PositionComponent& positionComponent, MovementComponent& movementComponent, float dt)
{
    // Cumulative fraction of one tile size for smooth path progressing
    static float cumulativeTileFraction{};

    // Distance to move this frame
    float frameDistance = (movementComponent.speed() * TextureData::TILE_SIZE) * dt;

    updateTileFraction(
        cumulativeTileFraction,
        frameDistance);

    updatePosition(
        &positionComponent,
        frameDistance,
        movementComponent.path().back().directionAccessed);

    // If next tile reached
    if (cumulativeTileFraction > TextureData::TILE_SIZE)
    {
        resetTileProgress(cumulativeTileFraction);

        adjustPosition(
            &positionComponent,
            movementComponent.path().back().tile->positionComponent.tilePosition());

        updatePath(&movementComponent);
    }

    // If target reached
    return isMovementComplete(&movementComponent);
}
