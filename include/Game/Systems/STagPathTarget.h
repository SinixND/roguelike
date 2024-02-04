#ifndef STAGPATHTARGET_H_20240204170113
#define STAGPATHTARGET_H_20240204170113

#include "System.h"

#include "CPosition.h"
#include "ECS.h"
#include "TPathTarget.h"
#include "TReachable.h"
#include "TUnderCursor.h"

class STagPathTarget
    : public snd::System<CPosition, TReachable>
{
public:
    void action(snd::EntityId entityId)
    {

        // Action
        if (ecs_->requestEntitySignature(entityId).test(TUnderCursor::getId()))
        {
            ecs_->assignComponent<TPathTarget>(entityId);
        }
        else
        {
            ecs_->removeComponent<TPathTarget>(entityId);
        }
    }

    STagPathTarget(snd::ECS* ecs)
        : snd::System<CPosition, TReachable>(ecs){};
};

#endif
