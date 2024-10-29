#include "Hero.h"

#include "DamageComponent.h"
#include "EnergyComponent.h"
#include "HealthComponent.h"
#include "HeroData.h"
#include "MovementComponent.h"

void Hero::init()
{
    movement = MovementComponent{20 * HeroData::BASE_AGILITY};
    energy = EnergyComponent{HeroData::BASE_AGILITY};

    health = HealthComponent{
        HeroData::BASE_HEALTH,
        HeroData::BASE_HEALTH_REGENERATION};

    damage = DamageComponent{HeroData::BASE_DAMAGE};

    visionRange = HeroData::VISION_RANGE;
}
