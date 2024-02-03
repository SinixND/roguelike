#ifndef SGENERATEMAP_H_20240128161046
#define SGENERATEMAP_H_20240128161046

#include "EntityId.h"
#include "SparseSet.h"
#include "System.h"

#include "CTileMap.h"
#include "ECS.h"
#include "MapGeneration.h"
#include "RuntimeDatabase.h"
#include <raylibEx.h>

class SGenerateMap
    : public snd::System<CTileMap>
{
public:
    void action(snd::EntityId entityId)
    {
        if (currentLevel_ == dtb::Configs::getCurrentLevel())
            return;

        ++currentLevel_;

        snd::SparseSet<Vector2Int, snd::EntityId>* tileMap{ecs_->getComponent<CTileMap>(entityId)->getTileMap()};

        if (currentLevel_ == 0)
        {
            setStartRoom(tileMap, ecs_);
        }
    }

    SGenerateMap(snd::ECS* ecs)
        : snd::System<CTileMap>(ecs)
    {
    }

private:
    static inline int currentLevel_{-1};
};

#endif
