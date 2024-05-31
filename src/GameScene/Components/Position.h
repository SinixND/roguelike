#ifndef IG20240227231122
#define IG20240227231122

#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

// Holds a world / render position and provides convenience functions to work with tile positions as well
class Position
{
    Vector2 position_{};

public:
    explicit Position(Vector2 const& position = {0, 0})
        : position_(position)
    {
    }

    explicit Position(Vector2I const& tilePosition)
    {
        changeTo(tilePosition);
    }

    Vector2 const& renderPosition() const { return position_; }

    Vector2I tilePosition() const
    {
        return UnitConversion::worldToPosition(position_);
    }

    void changeTo(Vector2 const& renderPosition)
    {
        position_ = renderPosition;
    }

    void changeTo(Vector2I const& tilePosition)
    {
        position_ = UnitConversion::positionToWorld(tilePosition);
    }

    void move(Vector2 const& offset)
    {
        position_ += offset;
    }
};

#endif