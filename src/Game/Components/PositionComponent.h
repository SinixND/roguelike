#ifndef IG20240227231122
#define IG20240227231122

#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>

//* Holds a world / render position and provides convenience functions to work with tile positions as well
class PositionComponent
{
    Vector2 position_{};

public:
    explicit PositionComponent(Vector2 const& position = {0, 0})
        : position_(position)
    {
    }

    explicit PositionComponent(Vector2I const& tilePosition)
        : position_(UnitConversion::tileToWorld(tilePosition))
    {
    }

    Vector2 const& worldPixel() const;
    Vector2I tilePosition() const;

    void changeTo(Vector2 const& worldPixel);
    void changeTo(Vector2I const& tilePosition);

    //* Returns if tilePosition changed
    bool move(Vector2 const& offset);

    // bool operator==(PositionComponent const& rhs) const;
    // bool operator==(Vector2 const& rhs) const;
    // bool operator==(Vector2I const& rhs) const;
};

#endif
