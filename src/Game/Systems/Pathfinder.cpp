#include "Pathfinder.h"
#include "Map.h"
#include "TileData.h"
#define DEBUG_PATHFINDER

#include "Directions.h"
#include "GameCamera.h"
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

#if defined(DEBUG) && defined(DEBUG_PATHFINDER)
#include "Debugger.h"
#include <format>
#include <string>
#endif

// RatedTile
// Heuristic used to rate tiles
// bias > 1: prioritize short path
// bias < 1: prioritize closer to target
float constexpr bias{2};

float RatedTile::rating() const
{
    return
        // Distance to target
        Vector2Length(distanceToTarget_)
        + bias * stepsNeeded();
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

// Pathfinder
// Adds three new neighbours for all tiles of current best rating to the rating list,
// then deleting current rating in map
// and restarting with new best rating,
// until target is found or no rating left to check
bool checkRatingList(
    int rating,
    std::forward_list<RatedTile>& ratedTiles,
    std::map<int, std::vector<RatedTile*>>& ratingList,
    std::unordered_set<Vector2I>& tilesToIgnore,
    Map& map,
    Vector2I const& target,
    GameCamera const& gameCamera,
    std::vector<Vector2I>& path)
{
    // Buffer rated tiles to allow neighbours with same rating
    std::vector<RatedTile*> tileList{ratingList[rating]};
    ratingList.erase(rating);

    // Check all tiles in vector for current best rating before choosing new best rating
    for (RatedTile* currentTile : tileList)
    // for (RatedTile* currentTile : ratingList[rating])
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
            if (
                !CheckCollisionPointRec(
                    UnitConversion::tileToScreen(
                        newTilePosition,
                        gameCamera.camera()),
                    gameCamera.viewportOnScreen()))
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
                currentTile->stepsNeeded() + 1,
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
            // - Enemy present
            if (
                (map.tiles().visibilityID(newTilePosition) == VisibilityID::invisible)
                || map.tiles().isSolid(newTilePosition)
                || !map.tiles().positions().contains(newTilePosition)
                || map.enemies().ids().contains(newTilePosition))
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

#if defined(DEBUG) && defined(DEBUG_PATHFINDER)
            DrawText(
                std::format(
                    "{:.0f}",
                    newRatedTile.rating())
                    .c_str(),
                UnitConversion::tileToScreen(
                    newTilePosition,
                    snx::debug::gcam().camera())
                        .x
                    + 10,
                UnitConversion::tileToScreen(
                    newTilePosition,
                    snx::debug::gcam().camera())
                        .y
                    + 10,
                10,
                WHITE);
#endif
        }
    }

    // All tiles with same rating have been checked -> remove key
    // ratingList.erase(rating);

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
            gameCamera,
            path))
    {
        return true;
    }

    return false;
}

std::vector<Vector2I> Pathfinder::findPath(
    Map& map,
    Vector2I const& start,
    Vector2I const& target,
    GameCamera const& gameCamera)
{
#if defined(DEBUG) && defined(DEBUG_PATHFINDER)
    BeginDrawing();
#endif

    std::vector<Vector2I> path{};

    // Return empty path if target is
    // - Is invisible
    // - Not accessible
    // - Not in map
    // - Equal to start
    if (
        (map.tiles().visibilityID(target) == VisibilityID::invisible)
        || map.tiles().isSolid(target)
        || !map.tiles().positions().contains(target)
        || Vector2Equals(start, target))
    {
        return path;
    }

    // Create tile to start
    RatedTile firstTile{
        start,
        target,
        0,
        nullptr};

    // Vector of rated tiles (persistent for ancestor pointers)
    std::forward_list<RatedTile> ratedTiles{firstTile};

    // Map of tile pointers, sorted by rating (lowest first)
    std::map<int, std::vector<RatedTile*>> ratingList{};

    ratingList[firstTile.rating()].push_back(&ratedTiles.front());

    // List of ignored tiles to avoid double checks
    std::unordered_set<Vector2I> tilesToIgnore{start};

    checkRatingList(
        firstTile.rating(),
        ratedTiles,
        ratingList,
        tilesToIgnore,
        map,
        target,
        gameCamera,
        path);

    // Path is either empty or has at least 2 entries (target and start)
#if defined(DEBUG) && defined(DEBUG_PATHFINDER)
    for (auto& position : path)
    {
        DrawCircleV(
            Vector2Add(
                UnitConversion::tileToScreen(
                    position,
                    snx::debug::gcam().camera()),
                Vector2{TileData::TILE_SIZE_HALF, TileData::TILE_SIZE_HALF}),
            5,
            ColorAlpha(
                GREEN,
                0.5));
    }
    EndDrawing();
#endif

    return path;
}