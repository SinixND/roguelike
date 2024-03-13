#ifndef IG20240211172120
#define IG20240211172120

#include "Graphic.h"
#include "Position.h"

struct Entity
{
    Position position;
    Graphic graphic;

    Entity(Position position, Graphic graphic)
        : position(position)
        , graphic(graphic){};
};

#endif