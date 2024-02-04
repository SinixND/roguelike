#ifndef PATHFINDING_H_20240128183417
#define PATHFINDING_H_20240128183417

#include "Utility.h"
#include <raylibEx.h>
#include <vector>

namespace snd
{
    class ECS;
}

struct SteppedTile
{
    Vector2Int position;
    Vector2Int direction = V_NODIR; // in which it was accessed
};

bool isInPositions(const Vector2Int& target, const std::vector<Vector2Int>& positions);
bool isInTileList(const Vector2Int& target, const std::vector<SteppedTile>& tiles);
bool isPositionInSteppedTiles(const Vector2Int& target, const std::vector<std::vector<SteppedTile>>& steppedTiles);

// Returns accessible positions (non-solid tiles)
std::vector<Vector2Int> filterTilesAccessible(snd::ECS* ecs);

bool isTileAccessible(const Vector2Int& target, snd::ECS* ecs);

std::vector<Vector2Int> filterTilesInRange(const Vector2Int& origin, size_t range, const std::vector<Vector2Int>& accessiblePositions);

std::vector<Vector2Int> filterTilesInRange(const Vector2Int& origin, size_t range, snd::ECS* ecs);

bool isTileInRange(const Vector2Int& origin, const Vector2Int& target, size_t range, snd::ECS* ecs);

std::vector<std::vector<SteppedTile>> filterTilesReachable(const Vector2Int& origin, size_t range, const std::vector<Vector2Int>& inRangePositions);

std::vector<std::vector<SteppedTile>> filterTilesReachable(const Vector2Int& origin, size_t range, snd::ECS* ecs);

bool isTileReachable(const Vector2Int& origin, const Vector2Int& target, size_t range, snd::ECS* ecs);

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
std::vector<SteppedTile> findPath(const Vector2Int& origin, const Vector2Int& target, size_t range, const std::vector<SteppedTile>& reachableTiles);

std::vector<SteppedTile> findPath(const Vector2Int& origin, const Vector2Int& target, size_t range, snd::ECS* ecs);

#endif