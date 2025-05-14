#include "PathSystem.h"

#include "ActionSystem.h"
#include "Enemies.h"
#include "Hero.h"
#include <vector>

namespace PathSystem
{
    void update(
        Hero& heroIO,
        Map const& map
    )
    {
        if (!heroIO.path.empty())
        {
            size_t pathSize{ heroIO.path.size() };

            if (pathSize == 2)
            {
                heroIO = ActionSystem::handleInputToAdjacentTarget(
                    heroIO,
                    map,
                    heroIO.path.rbegin()[1]
                );
            }

            if (pathSize > 2)
            {
                heroIO.path.pop_back();
            }

            if (pathSize < 2)
            {
                heroIO.path.clear();
            }
        }
    }
}
