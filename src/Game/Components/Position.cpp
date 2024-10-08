#include "Position.h"

#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>

Vector2 const& Position::worldPixel() const
{
    return position_;
}

Vector2I Position::tilePosition() const
{
    return UnitConversion::worldToTile(position_);
}

void Position::changeTo(Vector2 const& worldPixel)
{
    position_ = worldPixel;
}

void Position::changeTo(Vector2I const& tilePosition)
{
    position_ = UnitConversion::tileToWorld(tilePosition);
}

bool Position::move(Vector2 const& offset)
{
    Vector2I oldPosition{tilePosition()};

    position_ += offset;

    if (!Vector2Equals(oldPosition, tilePosition()))
    {
        return true;
    }

    return false;
}
