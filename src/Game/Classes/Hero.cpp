#include "Hero.h"

#include "Energy.h"
#include "HeroData.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"

RenderID Hero::renderID() const
{
    return renderID_;
}

void Hero::init()
{
    movement_.setSpeed(HeroData::HERO_SPEED);
    energy_.setMaxEnergy(HeroData::HERO_MAX_ENERGY);
    energy_.setRegenRate(HeroData::HERO_REGEN_RATE);
}
