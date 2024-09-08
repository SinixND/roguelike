#ifndef IG20240531143458
#define IG20240531143458

#include "Energy.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"

class Hero
{
    Movement movement_{};
    Energy energy_{};
    Position position_{};
    RenderID renderID_{RenderID::hero};

public:
    void init();

    Movement& movement();
    Energy& energy();
    Position& position();
    RenderID renderID();
};

#endif
