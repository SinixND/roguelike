#ifndef RENDER_H_20231217205005
#define RENDER_H_20231217205005

#include "ComponentManager.h"
#include "ComponentPosition.h"
#include "ComponentTexture.h"
#include "EntityId.h"
#include "System.h"
#include <iostream>
#include <raylib.h>

namespace snd
{
    class RenderSystem : public System<TextureComponent, PositionComponent>
    {
    public:
        void action(EntityId entityId)
        {
            // Get component pointers
            const auto& texture{
                componentManager_.retrieveFrom<TextureComponent>(entityId)};

            const auto& position{
                componentManager_.retrieveFrom<PositionComponent>(entityId)};

            // Action
            std::cout << "DUMMY: RenderSystem signature is: " << signature_ << "\n";
            std::cout << "DUMMY: Execute render action on entityId: " << entityId << "\n";
            std::cout << "DUMMY: Process texture " << texture->getId() << " and position " << position->getId() << "\n";
        }

        RenderSystem(ComponentManager& componentManager)
            : System<TextureComponent, PositionComponent>(componentManager)
        {
        }
    };
}

#endif