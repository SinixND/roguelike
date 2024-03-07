#ifndef _20240211230403
#define _20240211230403

#include "Entity.h"
#include "Graphic.h"
#include "Movement.h"
#include "Position.h"
#include "VisibilityID.h"

struct Unit : public Entity
{
    bool isSelected{false};
    Movement movement;
    const bool isSolid{true};
    VisibilityID visibilityID;
    int visionRange;

    Unit(Position position, Graphic graphic, Movement movement, VisibilityID visibilityID, int visionRange)
        : Entity(position, graphic)
        , movement(movement)
        , visibilityID(visibilityID)
        , visionRange(visionRange){};
};

#endif