#include "PositionComponent.h"
#include "Convert.h"
#include "raylibEx.h"
#include <raylib.h>

Vector2I PositionComponent::tilePosition() const
{
    return Convert::worldToTile(worldPixel);
}

void PositionComponent::changeTo(Vector2I const& tilePosition)
{
    worldPixel = Convert::tileToWorld(tilePosition);
}

void PositionComponent::move(Vector2 const& offset)
{
    worldPixel += offset;
}
