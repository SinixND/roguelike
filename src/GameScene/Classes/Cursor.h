#ifndef IG20240531185351
#define IG20240531185351

#include "Position.h"
#include "RenderID.h"
#include <raylib.h>

class Cursor
{
    Position position_{};
    RenderID renderID_{RenderID::cursor};
    bool isActive_{false};

public:
    Position& position() { return position_; }

    RenderID renderID() { return renderID_; }

    bool isActive() { return isActive_; }
    void toggle();

    void update(Camera2D const& camera, Vector2 const& heroPosition);
};

#endif