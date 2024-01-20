#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "Components.h"
#include "ECS.h"
#include "RuntimeDatabase.h"
#include "Utility.h"
#include <raylib.h>

class SControl
    : public snd::System<CPosition, CRenderOffset, TControlled>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};

        const auto& transform{ecs_->retrieveComponent<CRenderOffset>(entityId)->getTransform()};

        // Action
        position = Vector2Add(convertToTile(GetMousePosition()), transform);
    }

    SControl(snd::ECS* ecs)
        : snd::System<CPosition, CRenderOffset, TControlled>(ecs)
    {
    }
};

inline void renderAction(const Texture2D* texture, const Vector2& position, const Vector2& transform)
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

class SRenderMap
    : public snd::System<CTexture, CPosition, CRenderOffset, TRenderMap>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->retrieveComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->retrieveComponent<CRenderOffset>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderMap(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CRenderOffset, TRenderMap>(ecs){};
};

class SRenderObjects
    : public snd::System<CTexture, CPosition, CRenderOffset, TRenderObject>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->retrieveComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->retrieveComponent<CRenderOffset>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderObjects(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CRenderOffset, TRenderObject>(ecs){};
};

class SRenderUI
    : public snd::System<CTexture, CPosition, CRenderOffset, TRenderUI>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->retrieveComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->retrieveComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->retrieveComponent<CRenderOffset>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderUI(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CRenderOffset, TRenderUI>(ecs){};
};

// snd::System template
/*
    class CustomSystem
        : public snd::System<ComponentTypes>
    {
    public:
        void action(snd::EntityId entityId)
        {
            // Get components
            // auto& component{ecs_->retrieveComponent<ComponentType>(entityId)->component_};

            // Action

        }
        CustomSystem(snd::ECS* ecs)
            : snd::System<ComponentTypes>(ecs)
        {
        }
    }
*/

#endif
