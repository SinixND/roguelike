#include "PathSystem.h"

#include "ActionSystem.h"
#include "Enemies.h"
#include "Hero.h"

namespace PathSystem
{
    void update(
        Hero& heroIO,
        Map const& map
    )
    {
        if ( heroIO.path.size() > 1 )
        {
            heroIO = ActionSystem::handleInputToAdjacentTarget(
                heroIO,
                map,
                heroIO.path.rbegin()[1]
            );

            heroIO.path.pop_back();
        }

        else
        {
            heroIO.path.clear();
        }
    }
}
