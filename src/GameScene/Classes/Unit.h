#ifndef IG20240211230403
#define IG20240211230403

#include "Attack.h"
#include "Entity.h"
#include "Graphic.h"
#include "Movement.h"
#include "Position.h"
#include "VisibilityID.h"

class Unit : public Entity
{
public:
    bool isSelected{false};
    const bool isSolid{true};
    Movement movement;
    VisibilityID visibilityID;
    int visionRange;
    Attack attack;

    Unit(Position position, Graphic graphic, Movement movement, VisibilityID visibilityID, int visionRange, Attack attack)
        : Entity(position, graphic)
        , movement(movement)
        , visibilityID(visibilityID)
        , visionRange(visionRange)
        , attack(attack){};
};

#endif