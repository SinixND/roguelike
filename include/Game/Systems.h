#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "Components.h"
#include "ECS.h"
#include "RuntimeDatabase.h"
#include "Utility.h"
#include <raylib.h>
#include <raymath.h>

class SFollowMouse
    : public snd::System<CPosition, CRenderOffset, TMouseControlled>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};

        const auto& transform{ecs_->getComponent<CRenderOffset>(entityId)->getTransform()};

        // Action
        position = Vector2Add(convertToTile(GetMousePosition()), transform);
    }

    SFollowMouse(snd::ECS* ecs)
        : snd::System<CPosition, CRenderOffset, TMouseControlled>(ecs)
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
    : public snd::System<CTexture, CPosition, CRenderOffset, TRenderedAsMap>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->getComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->getComponent<CRenderOffset>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderMap(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CRenderOffset, TRenderedAsMap>(ecs){};
};

class SRenderObjects
    : public snd::System<CTexture, CPosition, CRenderOffset, TRenderedAsObject>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->getComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->getComponent<CRenderOffset>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderObjects(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CRenderOffset, TRenderedAsObject>(ecs){};
};

class SRenderUI
    : public snd::System<CTexture, CPosition, CRenderOffset, TRenderedAsUI>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->getComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->getComponent<CRenderOffset>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderUI(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CRenderOffset, TRenderedAsUI>(ecs){};
};

class STagUnderCursor
    : public snd::System<CPosition, TIsHoverable>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get entity
        snd::EntityId cursor{ecs_->getEntityWith<TIsCursor>()};

        // Get components
        auto& cursorPosition{ecs_->getComponent<CPosition>(cursor)->getPosition()};

        // Action
        if (Vector2Equals(ecs_->getComponent<CPosition>(entityId)->getPosition(), cursorPosition))
        {
            ecs_->assignComponent<TIsUnderCursor>(entityId);
        }
        else
        {
            ecs_->removeComponent<TIsUnderCursor>(entityId);
        }
    }

    STagUnderCursor(snd::ECS* ecs)
        : snd::System<CPosition, TIsHoverable>(ecs){};
};

class SSelection
    : public snd::System<TIsUnderCursor, TIsSelectable>
{
public:
    void action(snd::EntityId entityId)
    {
        // Action
        if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ecs_->toggleComponent<TIsSelected>(entityId);
        }
    }
    SSelection(snd::ECS* ecs)
        : snd::System<TIsUnderCursor, TIsSelectable>(ecs)
    {
    }
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
            auto* component{ecs_->getComponent<ComponentType>(entityId)};

            // Action

        }
        CustomSystem(snd::ECS* ecs)
            : snd::System<ComponentTypes>(ecs)
        {
        }
    };

*/

#endif
