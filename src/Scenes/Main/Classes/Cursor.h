#ifndef IG20240531185351
#define IG20240531185351

#include "PositionComponent.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <raylib.h>

class Cursor
{
public:
    PositionComponent position{};
    RenderID renderID{RenderID::CURSOR};
    bool isActive{false};

public:
    void toggle();

    void update(Camera2D const& camera, Vector2I const& heroPosition);
};

#endif
