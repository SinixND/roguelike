#ifndef SCONTROL_H_20240203213442
#define SCONTROL_H_20240203213442

#include "System.h"

#include "CPosition.h"
#include "ECS.h"
#include "TKeyControlled.h"
#include "Utility.h"
#include "raylibEx.h"
#include <raylib.h>

class SControl
    : public snd::System<CPosition, TKeyControlled>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};

        // Action
        switch (GetKeyPressed())
        {
        case KEY_W:
            position = Vector2Add(position, V_UP);
            break;

        case KEY_A:
            position = Vector2Add(position, V_LEFT);
            break;

        case KEY_S:
            position = Vector2Add(position, V_DOWN);
            break;

        case KEY_D:
            position = Vector2Add(position, V_RIGHT);
            break;

        default:
            break;
        }
    }

    SControl(snd::ECS* ecs)
        : snd::System<CPosition, TKeyControlled>(ecs)
    {
    }
};

#endif
