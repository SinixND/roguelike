#include "Hero.h"

#include "EnergyComponent.h"
#include "HeroData.h"
#include "MovementComponent.h"

void Hero::init()
{
    movement.setSpeed(20 * HeroData::HERO_BASE_AGILITY);
    energy.regenRate = HeroData::HERO_BASE_AGILITY;
    visionRange = HeroData::HERO_VISION_RANGE;
}
