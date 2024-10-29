#ifndef IG20240531143458
#define IG20240531143458

#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
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
    HealthComponent health{};
    DamageComponent damage{};

    int visionRange{};

public:
    void init();
};

#endif
