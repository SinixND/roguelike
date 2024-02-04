#ifndef SDESELECT_H_20240204184811
#define SDESELECT_H_20240204184811

#include "System.h"

#include "ECS.h"
#include "TSelected.h"
#include <raylib.h>

class SDeSelect
    : public snd::System<TSelected>
{
public:
    void action(snd::EntityId entityId)
    {
        // Action
        if (IsKeyPressed(KEY_ESCAPE))
        {
            ecs_->removeComponent<TSelected>(entityId);
        }
    }
    SDeSelect(snd::ECS* ecs)
        : snd::System<TSelected>(ecs)
    {
    }
};

#endif
