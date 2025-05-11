#include "PathSystem.h"

#include "ActionSystem.h"
#include "Enemies.h"
#include "Hero.h"
#include "raylibEx.h"
#include <vector>

namespace PathSystem
{
    std::vector<Vector2I> const& update(
        std::vector<Vector2I>& heroPath
    )
    {
        if ( heroPath.size() < 2 )
        {
            heroPath.clear();
        }

        return heroPath;
    }
}
