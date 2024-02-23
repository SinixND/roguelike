#ifndef _20240211230403
#define _20240211230403

#include "Movement.h"
#include "RenderID.h"
#include "raylibEx.h"

struct Unit
{
    Vector2Int position;
    RenderID renderID;
    bool isSelected;
    Movement movement;
};

#endif