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
    MovementComponent movement{20 * HeroData::BASE_AGILITY};
    EnergyComponent energy{HeroData::BASE_AGILITY};
    PositionComponent position{};
    HealthComponent health{
        HeroData::BASE_HEALTH,
        HeroData::BASE_HEALTH_REGENERATION};
    DamageComponent damage{HeroData::BASE_DAMAGE};

    int visionRange{HeroData::VISION_RANGE};
};

#endif
