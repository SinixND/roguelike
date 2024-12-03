#ifndef IG20240901143710
#define IG20240901143710

#include "raylibEx.h"
#include <vector>

struct Map;
class GameCamera;

class RatedTile
{
public:
    Vector2I tilePosition{};
    Vector2I distanceToTarget{};
    int stepsNeeded{};
    RatedTile* ancestor{nullptr};

public:
    RatedTile(
        Vector2I const& tilePosition,
        Vector2I const& target,
        int stepsNeeded,
        RatedTile* ancestor)
        : tilePosition(tilePosition)
        , distanceToTarget(Vector2Subtract(target, tilePosition))
        , stepsNeeded(stepsNeeded)
        , ancestor(ancestor)
    {
    }

    //* Heuristic used to rate tiles
    float rating() const;

    void reconstructPath(std::vector<Vector2I>& path);
};
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
        int maxRange = 0);
}

#endif
