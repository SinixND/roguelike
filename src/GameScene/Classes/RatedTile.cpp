#include "RatedTile.h"

#include "raylibEx.h"
#include <cstdlib>
#include <raymath.h>
#include <vector>

// Heuristic used to rate tiles
int RatedTile::rating() const
{
    return
        // Distance to target
        Vector2Length(distanceToTarget_);
}

void RatedTile::reconstructPath(std::vector<Vector2I>& path)
{
    // Add this to path
    path.push_back(tilePosition_);

    // Abort at root/start
    if (!ancestor_)
    {
        return;
    }

    // Add ancestor to path
    ancestor_->reconstructPath(path);
}