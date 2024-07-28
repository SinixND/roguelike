#include "Hero.h"

#include "Energy.h"
#include "Movement.h"
#include "raylibEx.h"

void Hero::init()
{
    movement_.setSpeed(20);
    energy_.setMaxEnergy(100);
    energy_.setRegenRate(10);
}

void Hero::triggerMovement(
    Vector2I const& direction,
    Movement& movement,
    Energy& energy)
{
    if (energy.consume(50))
    {
        movement.trigger(direction);
    }
}
