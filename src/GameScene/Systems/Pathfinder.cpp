#include "Pathfinder.h"

#include "Debugger.h"
#include "Directions.h"
#include "Tiles.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <raylib.h>
#include <raymath.h>
#include <map>
#include <unordered_set>
#include <vector>

class WeightedTile
{
    Vector2I tilePosition_{};
    int absoluteDistanceToTarget_{};
    int stepsNeeded_{};
    WeightedTile* ancestor_{};

public:
    WeightedTile(
        Vector2I const& tilePosition,
        Vector2I const& target,
        int stepsNeeded,
        WeightedTile* ancestor)
        : tilePosition_(tilePosition)
        , absoluteDistanceToTarget_(Vector2Distance(tilePosition, target))
        , stepsNeeded_(stepsNeeded)
        , ancestor_(ancestor)
    {
    }

    Vector2I const& tilePosition() const { return tilePosition_; }

    int stepsNeeded() const { return stepsNeeded_; }

    WeightedTile* ancestor() const { return ancestor_; }

    int weight() const { return (2 * absoluteDistanceToTarget_) + stepsNeeded_; }

    void reconstructPath(std::vector<Vector2I>& path) 
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
};

// Adds three new neighbours of all tiles in current best weight to weighting list, then deleting current weight in map and restarting with new best weight, until target is found or no weight  left to check
bool checkWeightingList(
    int weight,
    std::vector<WeightedTile>& weightedTiles,
    std::map<int, std::vector<WeightedTile*>>& weightingList,
    std::unordered_set<Vector2I>& tilesToIgnore,
    Tiles& map,
    Vector2I const& target,
    std::vector<Vector2I>& path)
{
    // Check all tiles in vector for current best weight before choosing new best weight
    for (WeightedTile* currentTile : weightingList[weight])
    {
#ifdef DEBUG
    BeginDrawing();
    BeginMode2D(snx::debug::cam());

    DrawCircleV(UnitConversion::tileToWorld(currentTile->tilePosition()), 2, RED);

    EndMode2D();
    EndDrawing();
#endif

        // Test all four directions for currentTile
        for (Vector2I const& direction : {
                Directions::V_UP,
                Directions::V_LEFT,
                Directions::V_DOWN,
                Directions::V_RIGHT
             })
        {
            // Calculate new tilePosition
            Vector2I newTilePosition{
                Vector2Add(
                    direction,
                    currentTile->tilePosition())};

            // Ignore ancestor
            if (
                currentTile->ancestor() 
                && Vector2Equals(
                    newTilePosition, 
                    currentTile->ancestor()->tilePosition()))
            {
                continue;
            }

        // Vector2I const& tilePosition,
        // Vector2I const& target,
        // int stepsNeeded,
        // WeightedTile* ancestor)
            WeightedTile newWeightedTile{
                newTilePosition,
                target,
                currentTile->stepsNeeded() + 1,
                currentTile};

            // If Target found
            if (
                Vector2Equals(
                    newTilePosition, 
                    target)
            )
            {
                newWeightedTile.reconstructPath(path);

                return true;
            }

            // Skip if is in tilesToIgnore
            if (tilesToIgnore.contains(newTilePosition))
            {
                continue;
            }

            // Skip if tile is invalid:
            // - Is invisible
            // - Not accessible
            // - Not in map
            if (
                (map.visibilityID(newTilePosition) == VisibilityID::invisible)
                || map.isSolid(newTilePosition)
                || !map.positions().contains(newTilePosition))
            {
                // Add to ignore set so it doesn't get checked again
                tilesToIgnore.insert(newTilePosition);

                continue;
            }

            // Create new weightedTile in vector
            weightedTiles.push_back(newWeightedTile);

            // Add newWeightedTile
            weightingList[newWeightedTile.weight()].push_back(&weightedTiles.back());
        }

        // Current tile has been checked
        tilesToIgnore.insert(currentTile->tilePosition());
    }

    // All tiles with same weight have been checked -> remove key
    weightingList.erase(weight);

    // Check new best weighted tiles
    if (!weightingList.empty())
    {
        checkWeightingList(
            weightingList.begin()->first,
            weightedTiles,
            weightingList,
            tilesToIgnore,
            map,
            target,
            path);
    }

    return false;
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
        return path;
    }

    // Create tile to start
    WeightedTile firstTile{
        start,
        target,
        0,
        nullptr};

    // Vector of weighted tiles (persistent for ancestor pointers)
    std::vector<WeightedTile> weightedTiles{firstTile};

    // Map of tile pointers, sorted by weight (lowest first)
    std::map<int, std::vector<WeightedTile*>> weightingList{};

    weightingList[firstTile.weight()].push_back(&weightedTiles.back());

    // List of ignored tiles to avoid double checks
    std::unordered_set<Vector2I> tilesToIgnore{};

    checkWeightingList(
        firstTile.weight(),
        weightedTiles,
        weightingList,
        tilesToIgnore,
        map,
        target,
        path);

    // Path is either empty or has at least 2 entries (target and start)
    return path;
}
