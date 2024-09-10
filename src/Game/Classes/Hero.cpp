#include "Hero.h"

#include "Energy.h"
#include "HeroData.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"

Movement& Hero::movement() { return movement_; }
Energy& Hero::energy() { return energy_; }
Position& Hero::position() { return position_; }
RenderID Hero::renderID() { return renderID_; }

void Hero::init()
{
    movement_.setSpeed(HeroData::HERO_SPEED);
    energy_.setMaxEnergy(HeroData::HERO_MAX_ENERGY);
    energy_.setRegenRate(HeroData::HERO_REGEN_RATE);
}
