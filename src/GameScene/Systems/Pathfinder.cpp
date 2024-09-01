#include "Pathfinder.h"

#include "Debugger.h"
#include "Directions.h"
#include "RotationMatrices.h"
#include "Tiles.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <set>
#include <unordered_set>
#include <vector>

class WeightedTile
{
    Vector2I tilePosition_{};
    Vector2I directionAccessedFrom_{};
    int absoluteDistanceToTarget_{};
    int stepsNeeded_{};

public:
    WeightedTile(
        Vector2I const& tilePosition,
        Vector2I const& directionAccessedFrom,
        Vector2I const& target,
        int stepsNeeded)
        : tilePosition_(tilePosition)
        , directionAccessedFrom_(directionAccessedFrom)
        , absoluteDistanceToTarget_(Vector2Distance(tilePosition, target))
        , stepsNeeded_(stepsNeeded)
    {
        if (Vector2Length(directionAccessedFrom) > 1)
        {
            [[maybe_unused]] int dbg{0};
        }
    }

    Vector2I const& tilePosition() const { return tilePosition_; }

    // AccessedFrom = Direction pointing to previous position
    Vector2I const& directionAccessedFrom() const { return directionAccessedFrom_; }

    int stepsNeeded() const { return stepsNeeded_; }

    int weight() const { return (2 * absoluteDistanceToTarget_) + stepsNeeded_; }
};

// Needed for sorting map of WeightedTile by weight
struct SortByWeight
{
    bool operator()(WeightedTile a, WeightedTile b) const
    {
        return a.weight() < b.weight();
    }
};

// Recursively checks neighbours, starting with the best/lowest weight, until target is found or no tiles left to check
bool checkNeighbours(
    Tiles& map,
    WeightedTile const currentTile,
    Vector2I const& target,
    std::multiset<WeightedTile, SortByWeight>& tilesToCheck,
    std::unordered_set<Vector2I>& tilesToIgnore,
    std::vector<Vector2I>& path)
{
#ifdef DEBUG
    BeginDrawing();
    BeginMode2D(snx::debug::cam());

    DrawCircleV(UnitConversion::tileToWorld(currentTile.tilePosition()), 2, RED);

    EndMode2D();
    EndDrawing();
#endif
    // Test all four directions for currentTile
    Vector2I mainDirection{
        GetMainDirection(
                currentTile.tilePosition(), 
                target)
    };

    for (Vector2I const& direction : {
             mainDirection,
             Vector2Transform(RotationMatrices::M_ROTATE_CW, mainDirection),
             Vector2Transform(RotationMatrices::M_ROTATE_CCW, mainDirection),
             Vector2Negate(mainDirection)
         })
    {
        // Ignore direction current tile is accessed from
        if (Vector2Equals(
                direction,
                currentTile.directionAccessedFrom()))
        {
#ifdef DEBUG
            if (target.x == 0 && target.y == 0)
            {
                // Break
            }
#endif
            continue;
        }

        // Calculate new position
        Vector2I newPosition{
            Vector2Add(
                currentTile.tilePosition(),
                direction)};

        // If Target found
        if (Vector2Equals(newPosition, target))
        {
            path.push_back(newPosition);
#ifdef DEBUG
            if (target.x == 0 && target.y == 0)
            {
                // Break
            }
#endif
            return true;
        }

        // Skip if is in tilesToIgnore
        if (tilesToIgnore.contains(newPosition))
        {
#ifdef DEBUG
            if (target.x == 0 && target.y == 0)
            {
                // Break
            }
#endif
            continue;
        }

        // Skip newPosition if not valid:
        // - Is invisible
        // - Not accessible
        // - Not in map
        if (
            (map.visibilityID(newPosition) == VisibilityID::invisible)
            || map.isSolid(newPosition)
            || !map.positions().contains(newPosition))
        {
            // Add to ignore set so it doesn't get checked again
            tilesToIgnore.insert(newPosition);
#ifdef DEBUG
            if (target.x == 0 && target.y == 0)
            {
                // Break
            }
#endif
            continue;
        }

        // Add tile to check
        std::cout << tilesToCheck.size() << "\n";
#ifdef DEBUG
        if (target.x == 0 && target.y == 0)
        {
            // Break
        }
#endif
        tilesToCheck.insert(
            WeightedTile(
                newPosition,
                Vector2Negate(direction),
                target,
                currentTile.stepsNeeded() + 1));
        std::cout << tilesToCheck.size() << "\n";
#ifdef DEBUG
        if (target.x == 0 && target.y == 0)
        {
            // Break
        }
#endif
    }

    // Current tile has been checked
    tilesToCheck.erase(currentTile);
    tilesToIgnore.insert(currentTile.tilePosition());

    // Get WeightedTile that has best/lowest weight
    WeightedTile tileToCheck{*tilesToCheck.begin()};

    // Remove if path not successful
    if (!checkNeighbours(
            map,
            tileToCheck,
            target,
            tilesToCheck,
            tilesToIgnore,
            path))
    {
        tilesToCheck.erase(tileToCheck);
#ifdef DEBUG
        if (target.x == 0 && target.y == 0)
        {
            // Break
        }
#endif
        return false;
    }

    // If target found
    path.push_back(tileToCheck.tilePosition());
#ifdef DEBUG
    if (target.x == 0 && target.y == 0)
    {
        // Break
    }
#endif
    return true;
}

std::vector<Vector2I> Pathfinder::findPath(
    Tiles& map,
    Vector2I const& start,
    Vector2I const& target)
{
    std::vector<Vector2I> path{};

    // Return empty path if target is
    // - Is invisible
    // - Not accessible
    // - Not in map
    // - Equal to start
    if (
        (map.visibilityID(target) == VisibilityID::invisible)
        || map.isSolid(target)
        || !map.positions().contains(target)
        || Vector2Equals(start, target))
    {
#ifdef DEBUG
        if (target.x == 0 && target.y == 0)
        {
            // Break
        }
#endif
        return path;
    }

    WeightedTile firstTile{
        start,
        Directions::V_NODIR,
        target,
        0};

    // Map of tiles, sorted by weight (lowest first)
    std::multiset<WeightedTile, SortByWeight> tilesToCheck{firstTile};

    std::unordered_set<Vector2I> tilesToIgnore{};

    if (checkNeighbours(
            map,
            *tilesToCheck.begin(),
            target,
            tilesToCheck,
            tilesToIgnore,
            path))
    {
        // Add starting tile if target was found to find starting direction for processing path
        path.push_back(start);
    }

    // Path is either empty or has at least 2 entries (target and start)
    return path;
}
