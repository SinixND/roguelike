#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "ComponentManager.h"
#include "Components.h"
#include "EntityId.h"
#include "System.h"
#include "Utility.h"
#include <vector>

namespace snd
{
    class RenderSystem
        : public System<TextureComponent, PositionComponent, DirectionComponent>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            const auto& texture{componentManager_.retrieveFrom<TextureComponent>(entityId)->texture_};

            const auto& position{componentManager_.retrieveFrom<PositionComponent>(entityId)->position_};

            const auto& direction{componentManager_.retrieveFrom<DirectionComponent>(entityId)->directionDeg_};

            // Get optional components
            auto* transformPtr{componentManager_.retrieveFrom<TransformComponent>(entityId)};
            Vector2 transform;

            if (!transformPtr)
            {
                transform = {0, 0};
            }
            else
            {
                transform = {transformPtr->transform_.x, transformPtr->transform_.y};
            }
            transform = coordinateToPosition(transform);

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
                    position.x + transform.x,
                    position.y + transform.y,
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

    class MouseCursorSystem
        : public System<MouseControlFlag, PositionComponent>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            auto& position{componentManager_.retrieveFrom<PositionComponent>(entityId)->position_};

            // Action
            position = coordinateToPosition(GetMousePosition());
        }

        MouseCursorSystem(ComponentManager& componentManager)
            : System<MouseControlFlag, PositionComponent>(componentManager)
        {
        }
    };

    class ControlSystem
        : public System<ControlFlag, PositionComponent, TransformComponent>
    {
    public:
        //* void action(EntityId entityId)
        //* {
        //* // Get components
        //* auto& position{componentManager_.retrieveFrom<PositionComponent>(entityId)->position_};
        //* auto* transforms{componentManager_.retrieveAll<TransformComponent>()};

        //* // Action
        //* // Move entity

        //* // Update all transforms
        //* for (const auto& transform : *transforms)
        //* {
        //* transform.transform_ = {offset.x, offset.y};
        //* }
        //* }

        ControlSystem(ComponentManager& componentManager)
            : System<ControlFlag, PositionComponent, TransformComponent>(componentManager)
        {
        }
    };

    // System template
    /*
        class CustomSystem
            : public System<ComponentTypes>
        {
        public:
            void action(EntityId entityId)
            {
                // Get components
                // auto& component{componentManager_.retrieveFrom<ComponentType>(entityId)->component_};

                // Action

            }
            CustomSystem(ComponentManager& componentManager)
                : System<ComponentTypes>(componentManager)
            {
            }
        }
    */
}

#endif