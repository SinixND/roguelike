#ifndef POSITION_H_20240128153104
#define POSITION_H_20240128153104

#include "Component.h"

struct Position
    : public snd::Component<Position>
{
    int x;
    int y;
};

#endif