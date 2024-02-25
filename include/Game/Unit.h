#ifndef _20240211230403
#define _20240211230403

#include "Graphic.h"
#include "Movement.h"
#include "raylibEx.h"

struct Unit
{
    Vector2i position;
    Graphic graphic;
    bool isSelected;
    Movement movement;
};

#endif