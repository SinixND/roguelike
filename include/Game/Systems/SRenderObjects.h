#ifndef SRENDEROBJECTS_H_20240128163910
#define SRENDEROBJECTS_H_20240128163910

#include "System.h"

#include "ActionRender.h"
#include "CPosition.h"
#include "CTexture.h"
#include "CTransform.h"
#include "ECS.h"
#include "TRenderedAsObject.h"

class SRenderObjects
    : public snd::System<CTexture, CPosition, CTransform, TRenderedAsObject>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->getComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->getComponent<CTransform>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderObjects(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CTransform, TRenderedAsObject>(ecs){};
};

#endif
