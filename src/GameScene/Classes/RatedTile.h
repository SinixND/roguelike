#ifndef IG20240903195341
#define IG20240903195341

#include "raylibEx.h"
#include <raymath.h>
#include <vector>

class RatedTile
{
    Vector2I tilePosition_{};
    Vector2I distanceToTarget_{};
    RatedTile* ancestor_{};

public:
    RatedTile(
        Vector2I const& tilePosition,
        Vector2I const& target,
        RatedTile* ancestor)
        : tilePosition_(tilePosition)
        , distanceToTarget_(Vector2Subtract(target, tilePosition))
        , ancestor_(ancestor)
    {
    }

    Vector2I const& tilePosition() const { return tilePosition_; }

    int stepsNeeded() const;

    RatedTile* ancestor() const { return ancestor_; }

    // Heuristic used to rate tiles
    int rating() const;

    void reconstructPath(std::vector<Vector2I>& path);
};

#endif