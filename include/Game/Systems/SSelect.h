#ifndef SSELECT_H_20240128164909
#define SSELECT_H_20240128164909

#include "System.h"

#include "ECS.h"
#include "TSelectable.h"
#include "TSelected.h"
#include "TUnderCursor.h"
#include <raylib.h>
#include <raylibEx.h>

class SSelect
    : public snd::System<TUnderCursor, TSelectable>
{
public:
    void action(snd::EntityId entityId)
    {
        // Action
        if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ecs_->toggleComponent<TSelected>(entityId);
        }
    }
    SSelect(snd::ECS* ecs)
        : snd::System<TUnderCursor, TSelectable>(ecs)
    {
    }
};

#endif
