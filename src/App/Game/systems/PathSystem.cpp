#include "PathSystem.h"

#include "ActionSystem.h"
#include "Convert.h"
#include "Hero.h"
#include <vector>

namespace PathSystem
{
    void update( Hero& heroIO )
    {
        if ( !heroIO.path.empty() )
        {
            size_t pathSize{ heroIO.path.size() };

            if (
                heroIO.path.rbegin()[1] == Convert::worldToTile( heroIO.position )
            )
            {
                heroIO.path.pop_back();
            }

            if ( pathSize < 2 )
            {
                heroIO.path.clear();
            }
        }
    }
}
