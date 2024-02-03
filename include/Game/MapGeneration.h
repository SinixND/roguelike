#ifndef MAPGENERATION_H_20240128151409
#define MAPGENERATION_H_20240128151409

#include "EntityId.h"
#include "SparseSet.h"
#include "Utility.h"
#include <raylibEx.h>

void addRoom(snd::SparseSet<Vector2Int, snd::EntityId>* tileMap, const Area& room);
void setTiles(snd::SparseSet<Vector2Int, snd::EntityId>* tileMap, const Area& area, TileType tileType);
snd::EntityId createNewMapTileEntitiy(Vector2Int position, snd::ECS* ecs);
void setStartRoom(snd::SparseSet<Vector2Int, snd::EntityId>* tileMap, snd::ECS* ecs);

#endif