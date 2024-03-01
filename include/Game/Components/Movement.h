#ifndef _20240218231113
#define _20240218231113

#include "TilePosition.h"
#include <cstddef>

struct Movement
{
    size_t range;
    TilePosition target;
    float speed;
};

#endif