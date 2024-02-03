#ifndef SFOLLOWMOUSE_H_20240128160349
#define SFOLLOWMOUSE_H_20240128160349

#include "System.h"

#include "CPosition.h"
#include "CTransform.h"
#include "ECS.h"
#include "TMouseControlled.h"
#include "Utility.h"
#include <raylibEx.h>
#include <raymath.h>

class SFollowMouse
    : public snd::System<CPosition, CTransform, TMouseControlled>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};

        const auto& transform{ecs_->getComponent<CTransform>(entityId)->getTransform()};

        // Action
        position = Vector2Add(convertToTile(GetMousePosition()), transform);
    }

    SFollowMouse(snd::ECS* ecs)
        : snd::System<CPosition, CTransform, TMouseControlled>(ecs)
    {
    }
};

#endif
