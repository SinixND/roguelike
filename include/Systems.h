#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "CONSTANTS.h"
#include "ComponentManager.h"
#include "Components.h"
#include "EntityId.h"
#include "System.h"
#include "Utility.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <vector>

namespace snd
{
    class RenderSystem
        : public System<TextureComponent, PositionComponent, RotationComponent, TransformComponent>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            const auto& texture{componentManager_.retrieveFrom<TextureComponent>(entityId)->texture_};

            const auto& position{componentManager_.retrieveFrom<PositionComponent>(entityId)->position_};

            const auto& rotation{componentManager_.retrieveFrom<RotationComponent>(entityId)->rotationDeg_};

            const auto& transform{componentManager_.retrieveFrom<TransformComponent>(entityId)->transform_};

            // Action
            Vector2 pixelPosition{
                Vector2Subtract(
                    transformedTileToPixel(
                        Vector2Subtract(
                            position,
                            transform)),
                    CONSTANTS::getInstance()->getTileSize())};

            DrawTexturePro(
                *texture,
                Rectangle{
                    0,
                    0,
                    float(texture->width),
                    float(texture->height)},
                Rectangle{
                    pixelPosition.x,
                    pixelPosition.y,
                    CONSTANTS::getInstance()->getTileSize().x,
                    CONSTANTS::getInstance()->getTileSize().y},
                Vector2{
                    0,
                    0},
                rotation,
                WHITE);
        }

        RenderSystem(ComponentManager& componentManager)
            : System<TextureComponent, PositionComponent, RotationComponent, TransformComponent>(componentManager)
        {
        }
    };

    class MouseCursorSystem
        : public System<MouseControlFlag, PositionComponent, TransformComponent>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            auto& position{componentManager_.retrieveFrom<PositionComponent>(entityId)->position_};

            const auto& transform{componentManager_.retrieveFrom<TransformComponent>(entityId)->transform_};

            // Action
            position = Vector2Subtract(pixelToTransformedTile(GetMousePosition()), transform);
        }

        MouseCursorSystem(ComponentManager& componentManager)
            : System<MouseControlFlag, PositionComponent, TransformComponent>(componentManager)
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
        //* auto* transforms{componentManager_.retrieveAll<SharedTransformComponent>()};

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
