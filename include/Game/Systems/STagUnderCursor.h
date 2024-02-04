#ifndef STAGUNDERCURSOR_H_20240128164342
#define STAGUNDERCURSOR_H_20240128164342

#include "System.h"

#include "CPosition.h"
#include "ECS.h"
#include "EntityId.h"
#include "THoverable.h"
#include "TIsCursor.h"
#include "TReachable.h"
#include "TUnderCursor.h"
#include <raymath.h>

class STagUnderCursor
    : public snd::System<CPosition, THoverable>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get entity
        snd::EntityId cursor{ecs_->getFirstEntityWith<TIsCursor>()};

        // Get components
        auto& cursorPosition{ecs_->getComponent<CPosition>(cursor)->getPosition()};

        // Action
        if (Vector2Equals(ecs_->getComponent<CPosition>(entityId)->getPosition(), cursorPosition))
        {
            ecs_->assignComponent<TUnderCursor>(entityId);
        }
        else
        {
            ecs_->removeComponent<TUnderCursor>(entityId);
        }
    }

    STagUnderCursor(snd::ECS* ecs)
        : snd::System<CPosition, THoverable>(ecs){};
};

#endif
