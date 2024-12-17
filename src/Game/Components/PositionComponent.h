#ifndef IG20240227231122
#define IG20240227231122

#include "Convert.h"
#include "raylibEx.h"
#include <raylib.h>

class PositionComponent
{
public:
    Vector2 worldPixel{};

public:
    PositionComponent() = default;

    explicit PositionComponent(Vector2 const& pixel)
        : worldPixel(pixel)
    {
    }

    explicit PositionComponent(Vector2I const& tilePosition)
        : worldPixel(Convert::tileToWorld(tilePosition))
    {
    }

    Vector2I tilePosition() const;

    void changeTo(Vector2I const& tilePosition);

    //* Returns if tilePosition changed
    void move(Vector2 const& offset);
};

#endif
