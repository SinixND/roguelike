#ifndef SRENDERMAP_H_20240128162733
#define SRENDERMAP_H_20240128162733

#include "System.h"

#include "ActionRender.h"
#include "CPosition.h"
#include "CTexture.h"
#include "CTransform.h"
#include "ECS.h"
#include "TRenderedAsMap.h"

class SRenderMap
    : public snd::System<CTexture, CPosition, CTransform, TRenderedAsMap>
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

    SRenderMap(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CTransform, TRenderedAsMap>(ecs){};
};

#endif
