#ifndef IG20240531194629
#define IG20240531194629

#include "Hero.h"

class InputHandler
{
    int keyCached{};

public:
    void getInput();
    void update(Hero* hero);
};

#endif
