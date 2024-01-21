#include "Utility.h"

#include "Components.h"
#include "ECS.h"
#include "RaylibExtention.h"
#include "RuntimeDatabase.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include <utility>
#include <vector>

const Vector2 convertToTile(const Vector2& pixelCoordinates)
{
    static Vector2 screenSize{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};

    if (IsWindowResized())
    {
        screenSize = {static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
    }

    Vector2 tileSize{dtb::Constants::tileSize_};

    // convert pixel coordinate to transformed tile coordinate (center of screen is tile (0,0))
    return Vector2{
        (std::floor((pixelCoordinates.x - ((screenSize.x / 2) - (tileSize.x / 2))) / tileSize.x)),
        (std::floor((pixelCoordinates.y - ((screenSize.y / 2) - (tileSize.y / 2))) / tileSize.y))};
}

const Vector2 convertToPixel(const Vector2& tileCoordinates)
{
    static Vector2 screenSize{static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};

    if (IsWindowResized())
    {
        screenSize = {static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())};
    }

    Vector2 tileSize{dtb::Constants::tileSize_};

    // convert transformed tile coordinate to pixel coordinate [ tile (0,0) is center of screen]
    return Vector2{
        (tileCoordinates.x * tileSize.x) + (screenSize.x / 2) + (tileSize.x),
        (tileCoordinates.y * tileSize.y) + (screenSize.y / 2) + (tileSize.y)};
}

// Pathfinder returns vector of positions to target
std::vector<Vector2> findPath(snd::ECS* ecs, Vector2& from, Vector2& target, size_t range)
{
    /*  directions in "**" are passed
        Every cell from 1+ checks the passed direction and only last (y = 0) checks its right direction
        Example shown for range of 3

    6   5   4   3   4   5   6
               *^*
    5   4   3   2   3   4   5
            ^  *^* *^*
    4   3*<*2   1 > 2 > 3   4
            ^   ^
    3*<*2*<*1 < F   1   2   3

    4   3   2   1   2   3   4

    5   4   3   2   3   4   5

    6   5   4   3   4   5   6
    */
    std::vector<Vector2> path{from};
    // Check if target equals root position
    if (Vector2Equals(target, from))
        return path;

    // Check if range is 0
    if (!range)
        return path;

    // Check if target is impassable
    auto* impassableTiles{ecs->getAllEntitiesWith<TIsImpassable>()};

    for (auto impassableTile : *impassableTiles)
    {
        if (Vector2Equals(target, ecs->getComponent<CPosition>(impassableTile)->getPosition()))
            return path;
    }

    // Start finding steps for path
    bool pathFound = false;

    using stepTile = std::pair<Vector2, Vector2>;

    std::vector<std::vector<stepTile>> steppedTiles{1, std::vector<stepTile>(1)};
    stepTile root{from, NODIR};
    steppedTiles[0][0] = root;

    // Step 1
    // Extend vector
    steppedTiles.push_back(std::vector<stepTile>(1));

    auto* passableTiles{ecs->getAllEntitiesWith<TIsPassable>()};

    // Test all four directions for first step
    for (auto dir : {VLEFT, VRIGHT, VUP, VDOWN})
    {
        // Set next stepped tile position
        auto nextTilePosition{Vector2Add(from, dir)};

        // Check if next tile is passable
        for (auto passableTile : *passableTiles)
        {
            auto passableTilePosition{ecs->getComponent<CPosition>(passableTile)->getPosition()};

            // If positions dont match: continue
            if (!Vector2Equals(
                    nextTilePosition,
                    passableTilePosition))
            {
                continue;
            }

            // Add stepped tile to container
            steppedTiles[1].push_back(std::make_pair(nextTilePosition, dir));

            // Check if stepped tile is equal to target
            if (Vector2Equals(
                    nextTilePosition,
                    target))
            {
                pathFound = true;
                path.push_back(steppedTiles[1].end()->first);
                break;
            }
            break;
        }
    }

    // Stop if range is 1
    if (range == 1) return path;

    // Remaining steps
    for (size_t stepLevel{2}; stepLevel <= range; ++stepLevel)
    {
        // Extend vector
        steppedTiles.push_back(std::vector<stepTile>(1));

        // Break if path found
        if (pathFound) break;

        size_t previousStepLevel{stepLevel - 1};
        size_t previousTilesCount{steppedTiles[previousStepLevel].size()};

        for (size_t n{1}; n < previousTilesCount; ++n)
        {
            stepTile steppedTile{steppedTiles[previousStepLevel][n]};

            // Check straight neighbours for all previously stepped tiles
            // Set next stepped tile position
            auto nextTilePosition{Vector2Add(steppedTile.first, steppedTile.second)};

            // Check if next tile is passable
            for (auto passableTile : *passableTiles)
            {
                auto passableTilePosition{ecs->getComponent<CPosition>(passableTile)->getPosition()};

                // If not passable continue
                if (!Vector2Equals(
                        nextTilePosition,
                        passableTilePosition))
                    continue;

                // Add passable tile to stepped tiles
                steppedTiles[stepLevel].push_back(std::make_pair(nextTilePosition, steppedTile.second));

                // Check if stepped tile is equal to target
                if (Vector2Equals(
                        nextTilePosition,
                        target))
                {
                    pathFound = true;
                    break;
                }
            }

            // Check orthogonal-right neighbour only for the last stepped tile of each quarter

            // If its not last tile of quarter (aka. remainder is not 0)
            if (n % ((previousTilesCount - 1) / 4))
                continue;

            // Break if path found
            if (pathFound) break;

            // Get orthogonal-right direction by rotating passed direction by 90 degrees
            Vector2 orthogonalRight{Vector2MatrixMultiply(steppedTile.second, Matrix2x2{0, -1, 1, 0})};
            Vector2Round(orthogonalRight);

            // Set next stepped tile position
            auto nextOrthogonalTilePosition{Vector2Add(steppedTile.first, orthogonalRight)};

            // Check if next tile is passable
            for (auto passableTile : *passableTiles)
            {
                auto passableTilePosition{ecs->getComponent<CPosition>(passableTile)->getPosition()};

                // If not passable continue
                if (!Vector2Equals(
                        nextOrthogonalTilePosition,
                        passableTilePosition))
                    continue;

                // Add passable tile to stepped tiles
                steppedTiles[stepLevel].push_back(std::make_pair(nextOrthogonalTilePosition, steppedTile.second));

                // Check if stepped tile is equal to target
                if (Vector2Equals(
                        nextTilePosition,
                        target))
                {
                    pathFound = true;
                    break;
                }
            }
        }
    }

    // Return empty path if no path found
    if (!pathFound) return path;

    // Reconstruct path from steps
    std::vector<Vector2> invertedPath;
    size_t stepsNeeded{steppedTiles.size()};
    Vector2 foundTarget{steppedTiles.end()->end()->first};

    // Add final position to invertedPath
    invertedPath.push_back(foundTarget);

    Vector2 currentStepLevelTile = foundTarget;

    // Add tile from previous step level if its neighbour to current step level tile (initialy this is the target)
    for (auto stepLevel{stepsNeeded - 2}; stepLevel > 0; --stepLevel)
    {
        // Check tiles from previous step level if its neighbour to current step level tile
        for (auto tile : steppedTiles[stepLevel])
        {
            Vector2 checkVector{Vector2Subtract(tile.first, currentStepLevelTile)};
            auto checkValue{abs(checkVector.x + checkVector.y)};

            // If subtracted Vectors have one coordinate 0 and the other +/- 1 then they are neighbours
            if (checkValue == 1)
            {
                invertedPath.push_back(tile.first);
                currentStepLevelTile = tile.first;
            }
        }
    }

    std::vector<Vector2> foundPath{invertedPath};
    // Reverse foundPath to get correct order
    std::reverse(foundPath.begin(), foundPath.end());

    // Add foundPath to path
    for (auto element : foundPath)
    {
        path.push_back(element);
    }

    // Return path (has only one element if no path found)
    return path;
}