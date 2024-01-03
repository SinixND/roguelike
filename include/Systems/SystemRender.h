#ifndef RENDER_H_20231217205005
#define RENDER_H_20231217205005

#include "ComponentManager.h"
#include "ComponentPosition.h"
#include "ComponentTexture.h"
#include "EntityId.h"
#include "System.h"
#include <iostream>

namespace snd
{
    class RenderSystem : public System<TextureComponent, PositionComponent>
    {
    public:
        void action(EntityId entity)
        {
            // Get component pointers
            auto texture{
                componentManager_.retrieveFrom<TextureComponent>(entity)};

            auto position{
                componentManager_.retrieveFrom<PositionComponent>(entity)};

            // Action
            std::cout << "DUMMY: RenderSystem signature is: " << signature_ << "\n";
            std::cout << "DUMMY: Execute render action on entity: " << entity << "\n";
            std::cout << "DUMMY: Process texture " << texture->getId() << " and position " << position->getId() << "\n";
        }

        RenderSystem(ComponentManager& componentManager)
            : System<TextureComponent, PositionComponent>(componentManager)
        {
        }
    };
}

#endif