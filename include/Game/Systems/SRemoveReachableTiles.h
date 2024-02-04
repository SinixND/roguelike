#ifndef SREMOVEREACHABLETILES_H_20240128165553
#define SREMOVEREACHABLETILES_H_20240128165553

#include "System.h"

#include "ECS.h"
#include "RuntimeDatabase.h"
#include "TReachable.h"
#include "TSelected.h"

class SRemoveReachableTiles
    : public snd::System<TReachable>
{
public:
    void action(snd::EntityId entityId)
    {
        // Skip if something is selected
        if (!ecs_->getAllEntitiesWith<TSelected>()->empty())
            return;

        dtb::State::hideTiles();

        // Action
        ecs_->removeEntity(entityId);
    }
    SRemoveReachableTiles(snd::ECS* ecs)
        : snd::System<TReachable>(ecs)
    {
    }
};

#endif
