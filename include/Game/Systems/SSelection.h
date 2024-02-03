#ifndef SSELECTION_H_20240128164909
#define SSELECTION_H_20240128164909

#include "System.h"

#include "ECS.h"
#include "TIsUnderCursor.h"
#include "TSelectable.h"
#include "TSelected.h"
#include <raylibEx.h>

class SSelection
    : public snd::System<TIsUnderCursor, TSelectable>
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
    SSelection(snd::ECS* ecs)
        : snd::System<TIsUnderCursor, TSelectable>(ecs)
    {
    }
};

#endif
