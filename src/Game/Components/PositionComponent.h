#ifndef IG20240227231122
#define IG20240227231122

#include "Convert.h"
#include "raylibEx.h"
#include <raylib.h>

struct PositionComponent
{
    Vector2 worldPixel{};

    PositionComponent() = default;

    explicit PositionComponent(Vector2 const& position)
        : worldPixel(position)
    {
    }

    explicit PositionComponent(Vector2I const& tilePosition)
        : worldPixel(Convert::tileToWorld(tilePosition))
    {
    }
};

namespace PositionModule
{
    Vector2I tilePosition(PositionComponent const& position);

    void changeTo(
        PositionComponent& position,
        Vector2I const& target);

    //* Returns if tilePosition changed
    void move(
        PositionComponent& position,
        Vector2 const& offset);
}

#endif
