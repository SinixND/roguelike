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
    class SRender
        : public System<CTexture, CPosition, CRotation, CTransform>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            const auto& texture{componentManager_.retrieveFrom<CTexture>(entityId)->getTexture()};

            const auto& position{componentManager_.retrieveFrom<CPosition>(entityId)->getPosition()};

            const auto& rotation{componentManager_.retrieveFrom<CRotation>(entityId)->getRotation()};

            const auto& transform{componentManager_.retrieveFrom<CTransform>(entityId)->getTransform()};

            // Action
            Vector2 tileSize{CONSTANTS::getInstance()->getTileSize()};
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

        SRender(ComponentManager& componentManager)
            : System<CTexture, CPosition, CRotation, CTransform>(componentManager)
        {
        }
    };

    class SMouseControl
        : public System<FMouseControlled, CPosition, CTransform>
    {
    public:
        void action(EntityId entityId)
        {
            // Get components
            auto& position{componentManager_.retrieveFrom<CPosition>(entityId)->getPosition()};

            const auto& transform{componentManager_.retrieveFrom<CTransform>(entityId)->getTransform()};

            // Action
            position = Vector2Add(pixelToTransformedTile(GetMousePosition()), transform);
        }

        SMouseControl(ComponentManager& componentManager)
            : System<FMouseControlled, CPosition, CTransform>(componentManager)
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
            auto rotation{componentManager_.retrieveFrom<CRotation>(entityId)};

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

        SRotation(ComponentManager& componentManager)
            : System<FKeyControlled, CRotation>(componentManager)
        {
        }
    };

    class SMovement
        : public System<FKeyControlled, CPosition, CRotation, CTransform>
    {
    public:
        void action(EntityId entityId)
        {
            if (!IsKeyPressed(KEY_SPACE))
                return;

            // Get components
            auto& position{componentManager_.retrieveFrom<CPosition>(entityId)->getPosition()};
            auto& direction{componentManager_.retrieveFrom<CRotation>(entityId)->getDirection()};
            auto* transforms{componentManager_.retrieveAll<CTransform>()};

            // Action
            // Move entity
            position = Vector2Add(position, direction);

            // Update all transforms
            for (auto& transform : *transforms)
            {
                transform.setTransform(position);
            }
        }

        SMovement(ComponentManager& componentManager)
            : System<FKeyControlled, CPosition, CRotation, CTransform>(componentManager)
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
