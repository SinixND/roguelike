#ifndef IG20240531191635
#define IG20240531191635

#include "Hero.h"

class World
{
    Hero hero_{};

public:
    Hero& hero() { return hero_; }
};

#endif