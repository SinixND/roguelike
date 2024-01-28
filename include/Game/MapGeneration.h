#ifndef MAPGENERATION_H_20240128151409
#define MAPGENERATION_H_20240128151409

#include "EntityId.h"
#include "SparseSet.h"
#include "Utility.h"
#include <raylib.h>

void addRoom(snd::SparseSet<Vector2, snd::EntityId>* tileMap, const Area& room);
void setTiles(snd::SparseSet<Vector2, snd::EntityId>* tileMap, const Area& area, TileType tileType);
snd::EntityId createNewMapTileEntitiy(Vector2 position, snd::ECS* ecs);
void setStartRoom(snd::SparseSet<Vector2, snd::EntityId>* tileMap, snd::ECS* ecs);

#endif