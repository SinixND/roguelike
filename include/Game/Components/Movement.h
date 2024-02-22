#ifndef _20240218231113
#define _20240218231113

#include "raylibEx.h"
#include <cstddef>

struct Movement
{
    size_t range;
    Vector2Int target;
    float speed;
};

#endif