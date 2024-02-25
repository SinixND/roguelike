#ifndef _20240128183417
#define _20240128183417

#include "Constants.h"
#include "TileMap.h"
#include <cstddef>
#include <raylibEx.h>
#include <vector>

struct SteppedPosition
{
    Vector2i position;
    Vector2i direction = V_NODIR; // in which it was accessed
};

using ClassifiedPositions = std::vector<std::vector<SteppedPosition>>;

bool isInPositions(
    const Vector2i& target,
    const std::vector<Vector2i>& positions);

bool isInTiles(
    const Vector2i& target,
    const std::vector<SteppedPosition>& tiles);

bool isInSteppedTiles(
    const Vector2i& target,
    const ClassifiedPositions& steppedTiles);

std::vector<Vector2i> filterNonSolidPositions(TileMap& tileMap);

std::vector<Vector2i> filterInRange(
    const std::vector<Vector2i>& nonSolidPositions,
    size_t range,
    const Vector2i& origin);

std::vector<Vector2i> filterInRange(
    TileMap& tileMap,
    size_t range,
    const Vector2i& origin);

bool isInRange(
    const Vector2i& target,
    size_t range,
    const Vector2i& origin,
    TileMap& tileMap);

ClassifiedPositions filterReachable(
    const std::vector<Vector2i>& inRangePositions,
    size_t range,
    const Vector2i& origin);

ClassifiedPositions filterReachable(
    TileMap& tileMap,
    size_t range,
    const Vector2i& origin);

bool isReachable(
    const Vector2i& target,
    size_t range,
    const Vector2i& origin,
    TileMap& tileMap);

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
std::vector<SteppedPosition> findPath(
    const ClassifiedPositions& reachableTiles,
    const Vector2i& target);

std::vector<SteppedPosition> findPath(
    TileMap& tileMap,
    const Vector2i& origin,
    const Vector2i& target,
    size_t range);

#endif