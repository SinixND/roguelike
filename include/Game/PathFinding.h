#ifndef PATHFINDING_H_20240128183417
#define PATHFINDING_H_20240128183417

#include "ECS.h"
#include <raylib.h>
#include <unordered_set>
#include <vector>

std::vector<Vector2> filterTilesAccessible();
std::vector<Vector2> filterTilesInRange();
std::vector<Vector2> filterTilesReachable();

std::vector<Vector2> findPath(const Vector2& from, const Vector2& target, size_t range, snd::ECS* ecs);
bool isTileAccessible(const Vector2& tile, const std::unordered_set<snd::EntityId>* impassableTiles, snd::ECS* ecs);

#endif