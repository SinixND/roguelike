#ifndef PATHFINDING_H_20240128183417
#define PATHFINDING_H_20240128183417

#include <raylib.h>
#include <vector>

namespace snd
{
    class ECS;
}

bool isPositionInList(const Vector2& target, const std::vector<Vector2>& positions);

// Returns accessible positions (non-solid tiles)
std::vector<Vector2> filterTilesAccessible(snd::ECS* ecs);

bool isTileAccessible(const Vector2& target, snd::ECS* ecs);

std::vector<Vector2> filterTilesInRange(const Vector2& origin, size_t range, const std::vector<Vector2>& accessiblePositions);

std::vector<Vector2> filterTilesInRange(const Vector2& origin, size_t range, snd::ECS* ecs);

bool isTileInRange(const Vector2& origin, const Vector2& target, size_t range, snd::ECS* ecs);

std::vector<Vector2> filterTilesReachable(const Vector2& origin, size_t range, const std::vector<Vector2>& inRangePositions);

std::vector<Vector2> filterTilesReachable(const Vector2& origin, size_t range, snd::ECS* ecs);

bool isTileReachable(const Vector2& origin, const Vector2& target, size_t range, snd::ECS* ecs);

// Pathfinder returns vector of positions from target to origin (excluded) if target is reachable
std::vector<Vector2> findPath(const Vector2& origin, const Vector2& target, size_t range, const std::vector<Vector2>& reachablePositions);

std::vector<Vector2> findPath(const Vector2& origin, const Vector2& target, size_t range, snd::ECS* ecs);

#endif