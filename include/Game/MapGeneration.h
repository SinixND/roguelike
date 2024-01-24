#ifndef MAPGENERATION_H_20240128151409
#define MAPGENERATION_H_20240128151409

#include "ContiguousMap.h"
#include "EntityId.h"
#include "Utility.h"
#include <raylib.h>

void addRoom(snd::ContiguousMap<Vector2, snd::EntityId>* tileMap, const Area& room);
void setTiles(snd::ContiguousMap<Vector2, snd::EntityId>* tileMap, const Area& area, TileType tileType);
snd::EntityId createNewMapTileEntitiy(Vector2 position);

#endif