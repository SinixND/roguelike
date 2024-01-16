#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "CONSTANTS.h"
#include "ECS.h"
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
    class SRender
        : public System<CTexture, CPosition, CRotation, CTransformation>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            const auto& texture{ecs_->retrieveComponent<CTexture>(entityId)->getTexture()};

            const auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};

            const auto& rotation{ecs_->retrieveComponent<CRotation>(entityId)->getRotation()};

            const auto& transform{ecs_->retrieveComponent<CTransformation>(entityId)->getTransform()};

            // Action
            Vector2 tileSize{CONSTANTS::get().getTileSize()};
            Vector2 tileCenter{Vector2Scale(tileSize, 0.5)};

            Vector2 pixelCoordinates{
                Vector2Subtract(
                    transformedTileToPixel(
                        Vector2Subtract(
                            position,
                            transform)),
                    tileSize)};

            std::cout << "Render " << entityId << ", Pos(" << position.x << ", " << position.y << ")\n";

            DrawTexturePro(
                *texture,
                Rectangle{
                    0,
                    0,
                    float(texture->width),
                    float(texture->height)},
                Rectangle{
                    pixelCoordinates.x,
                    pixelCoordinates.y,
                    tileSize.x,
                    tileSize.y},
                tileCenter,
                rotation,
                WHITE);
        }

        SRender(ECS* ecs)
            : System<CTexture, CPosition, CRotation, CTransformation>(ecs)
        {
        }
    };

    class SMouseControl
        : public System<FMouseControlled, CPosition, CTransformation>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};

            const auto& transform{ecs_->retrieveComponent<CTransformation>(entityId)->getTransform()};

            // Action
            position = Vector2Add(pixelToTransformedTile(GetMousePosition()), transform);
        }

        SMouseControl(ECS* ecs)
            : System<FMouseControlled, CPosition, CTransformation>(ecs)
        {
        }
    };

    class SRotation
        : public System<FKeyControlled, CRotation>
    {
    public:
        void action(EntityId entityId)
        {
            if (!IsKeyPressed(KEY_W) && !IsKeyPressed(KEY_A) && !IsKeyPressed(KEY_S) && !IsKeyPressed(KEY_D))
                return;

            // Get components
            auto rotation{ecs_->retrieveComponent<CRotation>(entityId)};

            // Action
            // Rotate entity
            switch (GetKeyPressed())
            {
            case KEY_W:
                rotation->setDirection(UP);
                break;
            case KEY_A:
                rotation->setDirection(LEFT);
                break;
            case KEY_S:
                rotation->setDirection(DOWN);
                break;
            case KEY_D:
                rotation->setDirection(RIGHT);
                break;
            }
        }

        SRotation(ECS* ecs)
            : System<FKeyControlled, CRotation>(ecs)
        {
        }
    };

    class SMovement
        : public System<FKeyControlled, CPosition, CRotation, CTransformation>
    {
    public:
        void action(EntityId entityId)
        {
            if (!IsKeyPressed(KEY_SPACE))
                return;

            // Get components
            auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};
            auto& direction{ecs_->retrieveComponent<CRotation>(entityId)->getDirection()};
            auto* transforms{ecs_->retrieveAllComponents<CTransformation>()};

            // Action
            Vector2 newPosition{Vector2Add(position, direction)};

            // Check for collision

            position = newPosition;

            // Update all transforms
            for (auto& transform : *transforms)
            {
                transform.setTransform(position);
            }
        }

        SMovement(ECS* ecs)
            : System<FKeyControlled, CPosition, CRotation, CTransformation>(ecs)
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
                // auto& component{ecs_->retrieveComponent<ComponentType>(entityId)->component_};

                // Action

            }
            CustomSystem(ECS* ecs)
                : System<ComponentTypes>(ecs)
            {
            }
        }
    */
}

#endif
