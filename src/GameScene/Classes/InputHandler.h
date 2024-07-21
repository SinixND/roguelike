#ifndef IG20240531194629
#define IG20240531194629

#include "Hero.h"
#include "HeroActionID.h"
#include <unordered_map>

class InputHandler
{
    HeroActionID heroAction_{HeroActionID::none};
    std::unordered_map<int, HeroActionID> keyToHeroActionID_{};

    void bind(int key, HeroActionID action);
    bool handleKey();
    bool handleGesture();

public:
    void setDefaultInputMappings();
    void handleInput();
    void update(Hero& hero);
};

#endif
