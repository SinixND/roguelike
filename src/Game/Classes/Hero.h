#ifndef IG20240531143458
#define IG20240531143458

#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "HeroData.h"
#include "PositionComponent.h"
#include "RenderId.h"
#include "TransformComponent.h"

struct Hero
{
    RenderId renderId{RenderId::HERO};
    TransformComponent transform{};
    EnergyComponent energy{
        HeroData::energyMax,
        HeroData::energyRegenBase};
    PositionComponent position{};
    HealthComponent health{
        HeroData::healthBase,
        HeroData::healthRegenBase};
    DamageComponent damage{HeroData::damageBase};

    int visionRange{HeroData::visionRange};
};

#endif
