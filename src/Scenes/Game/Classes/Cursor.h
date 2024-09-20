#ifndef IG20240531185351
#define IG20240531185351

#include "Position.h"
#include "RenderID.h"
#include "raylibEx.h"
#include <raylib.h>

class Cursor
{
    RenderID renderID_{RenderID::cursor};
    bool isActive_{false};

public:
    Position position_{};

public:
    RenderID renderID() const;
    bool isActive() const;
    void toggle();

    void update(Camera2D const& camera, Vector2I const& heroPosition);
};

#endif
