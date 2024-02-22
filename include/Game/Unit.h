#ifndef _20240211230403
#define _20240211230403

#include "GameObject.h"
#include "Movement.h"
#include <cstddef>

struct Unit : public GameObject
{
    bool isSelected;
    Movement movement;
};

#endif