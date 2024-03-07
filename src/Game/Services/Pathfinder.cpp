#include "Pathfinder.h"

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
Path findPath(
    const RangeSeparatedPositions& reachableTiles,
    const Vector2i& target)
{
    Path path{};

    // Check if target is reachable
    if (!TileMapFilters::isInSteppedTiles(target, reachableTiles))
        return path;

    Vector2i origin{reachableTiles.front().front().tilePosition};

    // Check if target equals root position
    if (Vector2Equals(target, origin))
        return path;

    // Find tiles in reachable positions, and recursively find neighbour until origin in next lower step level
    SteppedPosition currentStepLevelTile{};
    size_t stepsNeeded{};
    size_t maxRange{reachableTiles.size()};

    // Check if range is 0
    if (!maxRange)
        return path;

    // Find target tile in reachable positions
    for (size_t stepLevel{0}; stepLevel < maxRange; ++stepLevel)
    {
        for (const auto& tile : reachableTiles[stepLevel])
        {
            if (!Vector2Equals(tile.tilePosition, target))
                continue;

            currentStepLevelTile = tile;
            stepsNeeded = stepLevel;
        }
    }

    // Reconstruct path from steps

    // Add final/last position to path
    //* currentStepLevelTile.directionAccessed = V_NODIR;
    path.push_back(currentStepLevelTile);
    //* path.insert(path.begin(), currentStepLevelTile);

    // Push back position from previous step level to the path if it is a neighbour to current step level tile (which is the target initially)
    // Iterate all tiles that need one step less aka. are in previous step level
    for (auto stepLevel{stepsNeeded - 1}; stepLevel > 0; --stepLevel)
    {
        // Check tiles from previous step level if its neighbour to current step level tile
        for (auto tile : reachableTiles[stepLevel])
        {
            // CheckVector is delta between checked tile position and current step level position
            // CheckVector also is used to store the direction to next path tile
            Vector2i checkVector{Vector2Subtract(currentStepLevelTile.tilePosition, tile.tilePosition)};

            // Tiled length of checkVector needs to be 1 (then it is a neighbour)
            auto checkValue{Vector2Sum(checkVector)};

            // Check if tile is neighbour
            if (checkValue == 1)
            {
                // Add direction to next path tile to found tile
                //* tile.directionAccessed = checkVector;
                //* tile.directionAccessed = Vector2Subtract(path.back().tilePosition, tile.tilePosition);
                path.push_back(tile);
                currentStepLevelTile = tile;
            }
        }
    }

    return path;
}

Path findPath(
    TileMap& tileMap,
    const Vector2i& origin,
    const Vector2i& target,
    size_t range)
{
    return findPath(TileMapFilters::filterReachable(tileMap, range, origin), target);
}
