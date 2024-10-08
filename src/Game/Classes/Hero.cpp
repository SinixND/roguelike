#include "Hero.h"

#include "Energy.h"
#include "HeroData.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"
#include <utility>

void Hero::init()
{
    movement_.setSpeed(20 * HeroData::HERO_BASE_AGILITY);
    energy_.setRegenRate(HeroData::HERO_BASE_AGILITY);
}

RenderID Hero::renderID() const
{
    return renderID_;
}

Movement const& Hero::movement() const
{
    return movement_;
}

Movement& Hero::movement()
{
    return const_cast<Movement&>(std::as_const(*this).movement());
}

Energy const& Hero::energy() const
{
    return energy_;
}

Energy& Hero::energy()
{
    return const_cast<Energy&>(std::as_const(*this).energy());
}

Position const& Hero::position() const
{
    return position_;
}

Position& Hero::position()
{
    return const_cast<Position&>(std::as_const(*this).position());
}