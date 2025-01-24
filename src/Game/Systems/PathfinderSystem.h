#ifndef IG20240901143710
#define IG20240901143710

#include "raylibEx.h"
#include <vector>

struct Map;
struct GameCamera;

namespace PathfinderSystem
{
    //* Returns path from target (front()) to start (included, back())
    //* Starting tile needed for initial direction
    std::vector<Vector2I> findPath(
        Map const& map,
        Vector2I const& start,
        Vector2I const& target,
        GameCamera const& gameCamera,
        bool skipInvisibleTiles = true,
        int maxRange = 0
    );
}

#endif
