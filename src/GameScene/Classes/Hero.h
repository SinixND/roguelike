#ifndef IG20240531143458
#define IG20240531143458

#include "Movement.h"
#include "Position.h"
#include "RenderID.h"

class Hero
{
    Movement movement_{};
    Position position_{};
    RenderID renderID_{RenderID::HERO};

public:
    void init();

    Movement& movment() { return movement_; }
    Position& position() { return position_; };
    RenderID const& renderID() const { return renderID_; };
};

#endif