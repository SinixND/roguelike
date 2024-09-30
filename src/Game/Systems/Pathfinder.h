#ifndef IG20240901143710
#define IG20240901143710

#include "GameCamera.h"
#include "Map.h"
#include "raylibEx.h"
#include <raylib.h>
#include <vector>

class RatedTile
{
    Vector2I tilePosition_{};
    Vector2I distanceToTarget_{};
    int stepsNeeded_{};
    RatedTile* ancestor_{};

public:
    RatedTile(
        Vector2I const& tilePosition,
        Vector2I const& target,
        int stepsNeeded,
        RatedTile* ancestor)
        : tilePosition_(tilePosition)
        , distanceToTarget_(Vector2Subtract(target, tilePosition))
        , stepsNeeded_(stepsNeeded)
        , ancestor_(ancestor)
    {
    }

    Vector2I const& tilePosition() const
    {
        return tilePosition_;
    }

    int stepsNeeded() const
    {
        return stepsNeeded_;
    }

    RatedTile* ancestor() const
    {
        return ancestor_;
    }

    // Heuristic used to rate tiles
    float rating() const;

    void reconstructPath(std::vector<Vector2I>& path);
};

namespace Pathfinder
{
    std::vector<Vector2I> findPath(
        Map const& map,
        Vector2I const& start,
        Vector2I const& target,
        GameCamera const& gameCamera,
        int maxRange = 0);
}

#endif
