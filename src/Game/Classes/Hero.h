#ifndef IG20240531143458
#define IG20240531143458

#include "EnergyComponent.h"
#include "MovementComponent.h"
#include "PositionComponent.h"
#include "RenderID.h"

class Hero
{
public:
    RenderID renderID{RenderID::HERO};
    MovementComponent movement{};
    EnergyComponent energy{};
    PositionComponent position{};

public:
    void init();
};

#endif
