#ifndef _20240128183417
#define _20240128183417

#include "Constants.h"
#include "TileMap.h"
#include "TilePosition.h"
#include <cstddef>
#include <raylibEx.h>
#include <vector>

struct SteppedPosition
{
    TilePosition tilePosition;
    Vector2i directionAccessed = V_NODIR; // in which it was accessed
};

using RangeSeparatedPositions = std::vector<std::vector<SteppedPosition>>;

using Path = std::vector<SteppedPosition>;

bool isInPositions(
    const Vector2i& target,
    const std::vector<Vector2i>& positions);

bool isInTiles(
    const Vector2i& target,
    const std::vector<SteppedPosition>& tiles);

bool isInSteppedTiles(
    const Vector2i& target,
    const RangeSeparatedPositions& steppedTiles);

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

RangeSeparatedPositions filterReachable(
    const std::vector<Vector2i>& inRangePositions,
    size_t range,
    const Vector2i& origin);

RangeSeparatedPositions filterReachable(
    TileMap& tileMap,
    size_t range,
    const Vector2i& origin);

bool isReachable(
    const Vector2i& target,
    size_t range,
    const Vector2i& origin,
    TileMap& tileMap);

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
Path findPath(
    const RangeSeparatedPositions& reachableTiles,
    const Vector2i& target);

Path findPath(
    TileMap& tileMap,
    const Vector2i& origin,
    const Vector2i& target,
    size_t range);

#endif