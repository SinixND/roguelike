#ifndef IG20240531143458
#define IG20240531143458

#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "HeroData.h"
#include "MovementComponent.h"
#include "PositionComponent.h"
#include "RenderID.h"

struct Hero
{
    RenderID renderID{RenderID::HERO};
    MovementComponent movement{20 * HeroData::agilityBase};
    EnergyComponent energy{HeroData::agilityBase};
    PositionComponent position{};
    HealthComponent health{
        HeroData::healthBase,
        HeroData::healthRegenerationBase};
    DamageComponent damage{HeroData::damageBase};

    int visionRange{HeroData::visionRange};
};

#endif
