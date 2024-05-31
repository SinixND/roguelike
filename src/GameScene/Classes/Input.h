#ifndef IG20240531194629
#define IG20240531194629

#include "Hero.h"

class Input
{
    Hero* hero_{nullptr};

public:
    void init(Hero* hero);
    void check();
};

#endif
