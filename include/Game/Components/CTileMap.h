#ifndef CTILEMAP_H_20240128152626
#define CTILEMAP_H_20240128152626

#include "Component.h"
#include "ContiguousMap.h"
#include "EntityId.h"
#include <raylib.h>
#include <vector>

struct CTileMap
    : public snd::Component<CTileMap>
{
public:
    std::vector<snd::EntityId>* getTileEntities() { return tileMap_.getAllElements(); };
    snd::ContiguousMap<Vector2, snd::EntityId>* getTileMap() { return &tileMap_; }
    void addTileEntity(Vector2 position, snd::EntityId entityId) { tileMap_.insert(position, entityId); };

    CTileMap(){};

private:
    snd::ContiguousMap<Vector2, snd::EntityId> tileMap_{};
};

#endif