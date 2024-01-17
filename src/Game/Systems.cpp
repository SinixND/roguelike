#include "Systems.h"

#include "RuntimeDatabase.h"
#include "ECS.h"
#include "Utility.h"

namespace snd
{
    void SRender::action(EntityId entityId)
    {
        // Get components
        const auto& texture{ecs_->retrieveComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};
        const auto& rotation{ecs_->retrieveComponent<COrientation>(entityId)->getRotation()};
        const auto& transform{ecs_->retrieveComponent<CTransformation>(entityId)->getTransform()};

        // Action
        Vector2 tileSize{dtb::Constants::tileSize_};
        Vector2 tileCenter{Vector2Scale(tileSize, 0.5)};

        Vector2 pixelCoordinates{
            Vector2Subtract(
                convertToPixel(
                    Vector2Subtract(
                        position,
                        transform)),
                tileSize)};

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

    void SMouseControl::action(EntityId entityId)
    {
        // Get components
        auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};

        const auto& transform{ecs_->retrieveComponent<CTransformation>(entityId)->getTransform()};

        // Action
        position = Vector2Add(convertToTile(GetMousePosition()), transform);
    }

    void SRotation::action(EntityId entityId)
    {
        if (!IsKeyPressed(KEY_W) && !IsKeyPressed(KEY_A) && !IsKeyPressed(KEY_S) && !IsKeyPressed(KEY_D))
            return;

        // Get components
        auto rotation{ecs_->retrieveComponent<COrientation>(entityId)};

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

    void SMovement::action(EntityId entityId)
    {
        if (!IsKeyPressed(KEY_SPACE))
            return;

        // Get components
        auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};
        auto& direction{ecs_->retrieveComponent<COrientation>(entityId)->getDirection()};

        // Action
        Vector2 newPosition{Vector2Add(position, direction)};

        // Check for collision

        position = newPosition;

        // Update all transforms
        CTransformation::setTransform(position);
    }

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
