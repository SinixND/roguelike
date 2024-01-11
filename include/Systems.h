#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "Components.h"
#include "System.h"
#include "Utility.h"

namespace snd
{
    class RenderSystem : public System<TextureComponent, PositionComponent, DirectionComponent>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            const auto& texture{componentManager_.retrieveFrom<TextureComponent>(entityId)->texture_};

            const auto& position{componentManager_.retrieveFrom<PositionComponent>(entityId)->position_};

            const auto& direction{componentManager_.retrieveFrom<DirectionComponent>(entityId)->directionDeg_};

            // Action
            const auto& CONSTANTS{CONSTANTS::getInstance()};

            DrawTexturePro(
                *texture,
                Rectangle{
                    0,
                    0,
                    float(texture->width),
                    float(texture->height)},
                Rectangle{
                    position.x,
                    position.y,
                    CONSTANTS->getTileSize().x,
                    CONSTANTS->getTileSize().y},
                Vector2{
                    0,
                    0},
                direction,
                WHITE);
        }

        RenderSystem(ComponentManager& componentManager)
            : System<TextureComponent, PositionComponent, DirectionComponent>(componentManager)
        {
        }
    };

    class ControlSystem : public System<ControlFlag, PositionComponent>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            auto& position{componentManager_.retrieveFrom<PositionComponent>(entityId)->position_};

            // Action
            position = Utility::coordinateToPosition(GetMousePosition());
        }

        ControlSystem(ComponentManager& componentManager)
            : System<ControlFlag, PositionComponent>(componentManager)
        {
        }
    };
}

#endif