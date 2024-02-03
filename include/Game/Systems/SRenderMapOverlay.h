#ifndef SRENDERMAPOVERLAY_H_20240128164056
#define SRENDERMAPOVERLAY_H_20240128164056

#include "System.h"

#include "ActionRender.h"
#include "CPosition.h"
#include "CTexture.h"
#include "CTransform.h"
#include "ECS.h"
#include "TRenderedAsMapOverlay.h"

class SRenderMapOverlay
    : public snd::System<CTexture, CPosition, CTransform, TRenderedAsMapOverlay>
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

    SRenderMapOverlay(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CTransform, TRenderedAsMapOverlay>(ecs){};
};

#endif
