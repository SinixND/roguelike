#ifndef IG20240227231122
#define IG20240227231122

#include "Convert.h"
#include "raylibEx.h"
#include <raylib.h>

class PositionComponent
{
    Vector2 position_{};

public:
    PositionComponent() = default;

    explicit PositionComponent(Vector2 const& position)
        : position_(position)
    {
    }

    explicit PositionComponent(Vector2I const& tilePosition)
        : position_(Convert::tileToWorld(tilePosition))
    {
    }

    Vector2 const& worldPixel() const;
    Vector2I tilePosition() const;

    void changeTo(Vector2 const& worldPixel);
    void changeTo(Vector2I const& tilePosition);

    //* Returns if tilePosition changed
    bool move(Vector2 const& offset);
};

#endif
