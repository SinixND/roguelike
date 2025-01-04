#ifndef IG20240531185351
#define IG20240531185351

#include "PositionComponent.h"
#include "RenderId.h"

struct Camera2D;
typedef struct Camera2D Camera2D;
struct Vector2I;

struct Cursor
{
    PositionComponent position{};
    RenderId renderId{RenderId::CURSOR};
    bool isActive{false};
};

namespace CursorModule
{
    void toggle(Cursor& cursor);

    void update(
        Cursor& cursor,
        Camera2D const& camera,
        Vector2I const& heroPosition);
}
#endif
