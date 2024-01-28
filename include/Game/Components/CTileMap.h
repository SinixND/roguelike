#ifndef CTILEMAP_H_20240128152626
#define CTILEMAP_H_20240128152626

#include "Component.h"
#include "EntityId.h"
#include "SparseSet.h"
#include "Utility.h"
#include <raylib.h>
#include <vector>

struct CTileMap
    : public snd::Component<CTileMap>
{
public:
    snd::EntityId getTileEntity(Vector2 position) { return *tileMap_.get(position); };
    void addTileEntity(Vector2 position, snd::EntityId entityId) { tileMap_.insert(position, entityId); };
    snd::SparseSet<Vector2, snd::EntityId>* getTileMap() { return &tileMap_; }

    CTileMap(){};

private:
    snd::SparseSet<Vector2, snd::EntityId> tileMap_{};
};

#endif