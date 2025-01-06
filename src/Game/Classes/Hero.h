#ifndef IG20240531143458
#define IG20240531143458

#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "HeroData.h"
#include "MovementComponent.h"
#include "RenderId.h"
#include "TransformComponent.h"

struct Hero
{
    RenderId renderId{RenderId::HERO};
    TransformComponent transform{};
    MovementComponent movement{};
    EnergyComponent energy{
        HeroData::energyMax,
        HeroData::energyRegenBase};
    Vector2 position{};
    HealthComponent health{
        HeroData::healthBase,
        HeroData::healthRegenBase};
    DamageComponent damage{HeroData::damageBase};

    int visionRange{HeroData::visionRange};
};

#endif
