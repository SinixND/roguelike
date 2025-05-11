#include "PathSystem.h"

#include "ActionSystem.h"
#include "Hero.h"
#include <vector>

namespace PathSystem
{
    Hero const& update(
        Hero& hero
    )
    {
        if ( hero.path.size() < 2 )
        {
            hero.path.clear();
        }

        return hero;
    }
}
