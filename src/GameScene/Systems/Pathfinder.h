#ifndef IG20240901143710
#define IG20240901143710

#include "GameCamera.h"
#include "Tiles.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

namespace Pathfinder
{
    std::vector<Vector2I> findPath(
        Tiles& map,
        Vector2I const& start,
        Vector2I const& target,
        GameCamera const& gameCamera);
}

#endif
