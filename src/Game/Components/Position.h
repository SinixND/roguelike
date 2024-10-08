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
        : position_(UnitConversion::tileToWorld(tilePosition))
    {
    }

    Vector2 const& worldPixel() const;
    Vector2I tilePosition() const;

    void changeTo(Vector2 const& worldPixel);
    void changeTo(Vector2I const& tilePosition);

    // Returns if tilePosition changed
    bool move(Vector2 const& offset);
};

#endif
