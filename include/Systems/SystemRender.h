#ifndef RENDER_H_20231217205005
#define RENDER_H_20231217205005

#include "ComponentPosition.h"
#include "ComponentTexture.h"
#include "System.h"
#include <iostream>

namespace snd
{
    class RenderSystem : public System<RenderSystem>
    {
    public:
        RenderSystem()
        {
            signature_.set(TextureComponent::getId());
            signature_.set(PositionComponent::getId());
        };

        void action(TextureComponent& texture, PositionComponent& position)
        {
            std::cout << "DUMMY: Execute render action on texture " << texture.getId() << " and position " << position.getId() << "\n";
        }
    };
}

#endif