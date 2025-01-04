#include "PositionComponent.h"

#include "Convert.h"
#include "raylibEx.h"
#include <raylib.h>

Vector2I PositionModule::tilePosition(PositionComponent const& position)
{
    return Convert::worldToTile(position.worldPixel);
}

void PositionModule::changeTo(
    PositionComponent& position,
    Vector2I const& target)
{
    position.worldPixel = Convert::tileToWorld(target);
}

void PositionModule::move(
    PositionComponent& position,
    Vector2 const& offset)
{
    position.worldPixel += offset;
}

