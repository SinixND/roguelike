#ifndef IG20240531143458
#define IG20240531143458

#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "HeroData.h"
#include "RenderId.h"
#include "TransformComponent.h"

struct Hero
{
    RenderId renderId{RenderId::HERO};
    TransformComponent movement{HeroData::speed};
    EnergyComponent energy{HeroData::agilityBase};
    Vector2 position{};
    HealthComponent health{
        HeroData::healthBase,
        HeroData::healthBase,
        HeroData::healthRegenerationBase};
    DamageComponent damage{HeroData::damageBase};

    int visionRange{HeroData::visionRange};
};

#endif
