#include "Hero.h"

#include "Energy.h"
#include "Movement.h"

void Hero::init()
{
    movement_.setSpeed(20);
    energy_.setMaxEnergy(100);
    energy_.setRegenRate(10);
}
