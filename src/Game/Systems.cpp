#include "Systems.h"

#include "ECS.h"
#include "RuntimeDatabase.h"
#include "Utility.h"
#include <raylib.h>

namespace snd
{
    void SMouseControl::action(EntityId entityId)
    {
        // Get components
        auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};

        const auto& transform{ecs_->retrieveComponent<CRenderOffset>(entityId)->getTransform()};

        // Action
        position = Vector2Add(convertToTile(GetMousePosition()), transform);
    }

    void SMovement::action(EntityId entityId)
    {
        if (!IsKeyPressed(KEY_SPACE))
            return;

        // Get components
        auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};
        auto& direction{ecs_->retrieveComponent<CDirection>(entityId)->getDirection()};

        // Action
        Vector2 newPosition{Vector2Add(position, direction)};

        // Check for collision

        position = newPosition;

        // Update all transforms
        CRenderOffset::setTransform(position);
    }

    void renderAction(const Texture2D* texture, const Vector2& position, const Vector2& transform)
    {
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
            0,
            WHITE);
    }

    void SRenderMap::action(EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->retrieveComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->retrieveComponent<CRenderOffset>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    void SRenderObjects::action(EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->retrieveComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->retrieveComponent<CRenderOffset>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    void SRenderUI::action(EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->retrieveComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->retrieveComponent<CRenderOffset>(entityId)->getTransform()};

        renderAction(texture, position, transform);
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
