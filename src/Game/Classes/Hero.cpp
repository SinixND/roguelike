#include "Hero.h"

#include "Energy.h"
#include "HeroData.h"
#include "Movement.h"
#include "RenderID.h"

RenderID Hero::renderID() const
{
    return renderID_;
}

void Hero::init()
{
    movement_.setSpeed(10 * HeroData::HERO_BASE_AGILITY);
    energy_.setRegenRate(HeroData::HERO_BASE_AGILITY);
}
