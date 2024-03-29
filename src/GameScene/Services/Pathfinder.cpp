#include "Pathfinder.h"

#include "TileMap.h"
#include "TileMapFilters.h"
#include "raylibEx.h"
#include <cstddef>

// Pathfinder returns vector of positions from target to origin (excluded) if
// target is reachable
Path findPath(RangeSeparatedTiles const& reachableTiles, Vector2i target)
{
    Path path{};

    // Check if target is reachable
    if (!TileMapFilters::isInRangeSeparatedTiles(target, reachableTiles))
        return path;

    Vector2i origin{
      reachableTiles.front().front().tile->transform.tilePosition()};

    // Check if target equals root position
    if (Vector2Equals(target, origin)) return path;

    // Find tiles in reachable positions, and recursively find neighbour until
    // origin in next lower step level
    SteppedTile currentStepLevelTile{};
    size_t stepsNeeded{};
    size_t maxRange{reachableTiles.size()};

    // Check if range is 0
    if (!maxRange) return path;

    // Find target tile in reachable positions
    for (size_t stepLevel{0}; stepLevel < maxRange; ++stepLevel) {
        for (auto const& steppedTile : reachableTiles[stepLevel]) {
            if (!Vector2Equals(
                  steppedTile.tile->transform.tilePosition(),
                  target))
                continue;
            currentStepLevelTile = steppedTile;
            stepsNeeded = stepLevel;
        }
    }

    // Reconstruct path from steps
    // Add final/last position to path
    path.push_back(currentStepLevelTile);

    // Push back position from previous step level to the path if it is a
    // neighbour to current step level tile (which is the target initially)
    // Iterate all tiles that need one step less aka. are in previous step
    // level
    for (auto stepLevel{stepsNeeded - 1}; stepLevel > 0; --stepLevel) {

        // Check tiles from previous step level if its neighbour to current
        // step level tile
        for (auto steppedTile : reachableTiles[stepLevel]) {

            // CheckVector is delta between checked tile position and current
            // step level position CheckVector also is used to store the
            // direction to next path tile
            Vector2i checkVector{Vector2Subtract(
              currentStepLevelTile.tile->transform.tilePosition(),
              steppedTile.tile->transform.tilePosition())};

            // Tiled length of checkVector needs to be 1 (then it is a
            // neighbour)
            auto checkValue{Vector2Sum(checkVector)};

            // Check if tile is neighbour
            if (checkValue == 1) {

                // Add direction to next path tile to found tile
                path.push_back(steppedTile);
                currentStepLevelTile = steppedTile;
            }
        }
    }

    return path;
}

Path findPath(TileMap& tileMap, Vector2i origin, Vector2i target, size_t range)
{
    return findPath(
      TileMapFilters::filterMovable(tileMap, range, origin),
      target);
}
