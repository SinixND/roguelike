#include "PositionComponent.h"
#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>

Vector2 const& PositionComponent::worldPixel() const
{
    return position_;
}

Vector2I PositionComponent::tilePosition() const
{
    return UnitConversion::worldToTile(position_);
}

void PositionComponent::changeTo(Vector2 const& worldPixel)
{
    position_ = worldPixel;
}

void PositionComponent::changeTo(Vector2I const& tilePosition)
{
    position_ = UnitConversion::tileToWorld(tilePosition);
}

bool PositionComponent::move(Vector2 const& offset)
{
    Vector2I oldPosition{tilePosition()};

    position_ += offset;

    if (!Vector2Equals(oldPosition, tilePosition()))
    {
        return true;
    }

    return false;
}
