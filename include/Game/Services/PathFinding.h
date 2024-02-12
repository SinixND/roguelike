#ifndef _20240128183417
#define _20240128183417

#include "TileMap.h"
#include "Utility.h"
#include <cstddef>
#include <raylibEx.h>
#include <vector>

struct SteppedPosition
{
    Vector2Int position;
    Vector2Int direction = V_NODIR; // in which it was accessed
};

bool isInPositions(const Vector2Int& target, const std::vector<Vector2Int>& positions);

bool isInTiles(const Vector2Int& target, const std::vector<SteppedPosition>& tiles);

bool isInSteppedTiles(const Vector2Int& target, const std::vector<std::vector<SteppedPosition>>& steppedTiles);

std::vector<Vector2Int> filterNonSolidPositions(TileMap& tileMap);

std::vector<Vector2Int> filterInRange(const std::vector<Vector2Int>& nonSolidPositions, size_t range, const Vector2Int& origin);

std::vector<Vector2Int> filterInRange(TileMap& tileMap, size_t range, const Vector2Int& origin);

bool isInRange(const Vector2Int& target, size_t range, const Vector2Int& origin, TileMap& tileMap);

std::vector<std::vector<SteppedPosition>> filterReachable(const std::vector<Vector2Int>& inRangePositions, size_t range, const Vector2Int& origin);

std::vector<std::vector<SteppedPosition>> filterReachable(TileMap& tileMap, size_t range, const Vector2Int& origin);

bool isReachable(const Vector2Int& target, size_t range, const Vector2Int& origin, TileMap& tileMap);

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
std::vector<SteppedPosition> findPath(const std::vector<std::vector<SteppedPosition>>& reachableTiles, const Vector2Int& origin, const Vector2Int& target, size_t range);

std::vector<SteppedPosition> findPath(TileMap& tileMap, const Vector2Int& origin, const Vector2Int& target, size_t range);

#endif