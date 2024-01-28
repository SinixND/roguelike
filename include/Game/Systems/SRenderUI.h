#ifndef SRENDERUI_H_20240128164109
#define SRENDERUI_H_20240128164109

#include "System.h"

#include "ActionRender.h"
#include "CPosition.h"
#include "CTexture.h"
#include "CTransform.h"
#include "ECS.h"
#include "TRenderedAsUI.h"

class SRenderUI
    : public snd::System<CTexture, CPosition, CTransform, TRenderedAsUI>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->getComponent<CTexture>(entityId)->get()};
        const auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->getComponent<CTransform>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderUI(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CTransform, TRenderedAsUI>(ecs){};
};

#endif