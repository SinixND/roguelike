#include "Pathfinder.h"

#include "Directions.h"
#include "RatedTile.h"
#include "Tiles.h"
#include "UnitConversion.h"
#include "VisibilityID.h"
#include "raylibEx.h"
#include <forward_list>
#include <map>
#include <raylib.h>
#include <raymath.h>
#include <unordered_set>
#include <vector>

// Adds three new neighbours for all tiles of current best rating to the rating list,
// then deleting current rating in map
// and restarting with new best rating,
// until target is found or no rating left to check
bool checkRatingList(
    int rating,
    std::forward_list<RatedTile>& ratedTiles,
    std::map<int, std::vector<RatedTile*>>& ratingList,
    std::unordered_set<Vector2I>& tilesToIgnore,
    Tiles& map,
    Vector2I const& target,
    RectangleEx const& mapPanel,
    Camera2D const& camera,
    std::vector<Vector2I>& path)
{
    // Check all tiles in vector for current best rating before choosing new best rating
    for (auto currentTile : ratingList[rating])
    {
        // Test all four directions for currentTile
        for (Vector2I const& direction : {
                 Directions::V_RIGHT,
                 Directions::V_DOWN,
                 Directions::V_LEFT,
                 Directions::V_UP})
        {
            // Calculate new tilePosition
            Vector2I newTilePosition{
                Vector2Add(
                    direction,
                    currentTile->tilePosition())};

            // Needs to be in map panel
            if (!CheckCollisionPointRec(UnitConversion::tileToScreen(newTilePosition, camera), mapPanel))
            {
                continue;
            }

            // Ignore ancestor
            if (
                currentTile->ancestor()
                && Vector2Equals(
                    newTilePosition,
                    currentTile->ancestor()->tilePosition()))
            {
                continue;
            }

            RatedTile newRatedTile{
                newTilePosition,
                target,
                currentTile};

            // If Target found
            if (
                Vector2Equals(
                    newTilePosition,
                    target))
            {
                newRatedTile.reconstructPath(path);

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
                // Invalid! Add to ignore set so it doesn't get checked again
                tilesToIgnore.insert(newTilePosition);

                continue;
            }

            // Create new ratedTile in vector
            ratedTiles.push_front(newRatedTile);

            // Add newRatedTile
            ratingList[newRatedTile.rating()].push_back(&ratedTiles.front());

            // Valid! Add to ignore set so it doesn't get checked again
            tilesToIgnore.insert(newTilePosition);
        }
    }

    // All tiles with same rating have been checked -> remove key
    ratingList.erase(rating);

    // Check new best rated tiles
    if (
        !ratingList.empty()
        && checkRatingList(
            ratingList.begin()->first,
            ratedTiles,
            ratingList,
            tilesToIgnore,
            map,
            target,
            mapPanel,
            camera,
            path))
    {
        return true;
    }

    return false;
}

std::vector<Vector2I> Pathfinder::findPath(
    Tiles& map,
    Vector2I const& start,
    Vector2I const& target,
    RectangleEx const& mapPanel,
    Camera2D const& camera)
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
    RatedTile firstTile{
        start,
        target,
        nullptr};

    // Vector of rated tiles (persistent for ancestor pointers)
    std::forward_list<RatedTile> ratedTiles{firstTile};

    // Map of tile pointers, sorted by rating (lowest first)
    std::map<int, std::vector<RatedTile*>> ratingList{};

    ratingList[firstTile.rating()].push_back(&ratedTiles.front());

    // List of ignored tiles to avoid double checks
    std::unordered_set<Vector2I> tilesToIgnore{};

    checkRatingList(
        firstTile.rating(),
        ratedTiles,
        ratingList,
        tilesToIgnore,
        map,
        target,
        mapPanel,
        camera,
        path);

    // Path is either empty or has at least 2 entries (target and start)
    return path;
}
