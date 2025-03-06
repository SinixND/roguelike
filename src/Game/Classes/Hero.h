#ifndef IG20240531143458
#define IG20240531143458

#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "ExperienceComponent.h"
#include "HealthComponent.h"
#include "HeroData.h"
#include "MovementComponent.h"
#include "RenderId.h"
#include "TransformComponent.h"

struct Hero
{
    MovementComponent movement{};

    TransformComponent transform{};

    EnergyComponent energy{
        HeroData::energyRegenBase,
        HeroData::energyMax
    };

    HealthComponent health{
        HeroData::healthBase,
        HeroData::healthRegenBase
    };

    ExperienceComponent experience{};

    DamageComponent damage{ HeroData::damageBase };

    Vector2 position{};

    RenderId renderId{ RenderId::HERO };

    int visionRange{ HeroData::visionRange };
};

#endif
