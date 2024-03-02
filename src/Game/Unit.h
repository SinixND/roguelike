#ifndef _20240211230403
#define _20240211230403

#include "Graphic.h"
#include "Movement.h"
#include "Position.h"

struct Unit
{
    Position position;
    Graphic graphic;
    bool isSelected;
    Movement movement;
};

#endif