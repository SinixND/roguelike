#ifndef IG20240531143458
#define IG20240531143458

#include "Energy.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"
#include "raylibEx.h"

class Hero
{
    Movement movement_{};
    Energy energy_{};
    Position position_{};
    RenderID renderID_{RenderID::hero};

public:
    void init();

    Movement& movement() { return movement_; }
    Energy& energy() { return energy_; }
    Position& position() { return position_; };
    RenderID renderID() { return renderID_; };

    void triggerMovement(
        Vector2I const& direction, 
        Movement& movement, 
        Energy& energy);
};

#endif