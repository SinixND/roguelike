#include "Hero.h"

#include "Energy.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"

Movement& Hero::movement() { return movement_; }
Energy& Hero::energy() { return energy_; }
Position& Hero::position() { return position_; }
RenderID Hero::renderID() { return renderID_; }

void Hero::init()
{
    movement_.setSpeed(20);
    energy_.setMaxEnergy(100);
    energy_.setRegenRate(10);
}
