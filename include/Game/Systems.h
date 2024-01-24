#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "Components.h"
#include "ECS.h"
#include "EntityId.h"
#include "RuntimeDatabase.h"
#include "System.h"
#include "Utility.h"
#include <cstdlib>
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

class SRenderMapOverlay
    : public snd::System<CTexture, CPosition, CRenderOffset, TRenderedAsMapOverlay>
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

    SRenderMapOverlay(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CRenderOffset, TRenderedAsMapOverlay>(ecs){};
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

static bool tilesShown{false};

class SShowReachableTiles
    : public snd::System<TIsSelected, CPosition, CRangeMovement>
{
public:
    void action(snd::EntityId entityId)
    {
        if (tilesShown)
            return;

        tilesShown = true;

        // Get components
        auto position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        auto moveRange{ecs_->getComponent<CRangeMovement>(entityId)->getMovementRange()};

        // Action
        for (int x{-moveRange}; x <= moveRange; ++x)
        {
            for (int y{-moveRange}; y <= moveRange; ++y)
            {
                if ((abs(x) + abs(y)) > moveRange)
                    continue;

                Vector2 newTilePosition{
                    Vector2Add(
                        position,
                        Vector2{
                            static_cast<float>(x),
                            static_cast<float>(y)})};

                //* DEBUG START
                if (newTilePosition.x == -1 && newTilePosition.y == -2)
                {
                    [[maybe_unused]] auto dbg{0};
                }
                //* DEBUG END

                auto path{findPath(ecs_, position, newTilePosition, moveRange)};

                if (path.size() < 2)
                    continue;

                auto newMoveableTile{ecs_->createEntity()};

                ecs_->assignComponent<CPosition>(newMoveableTile, newTilePosition);
                ecs_->assignComponent<CTexture>(newMoveableTile, dtb::Textures::get(REACHABLE_TILE));
                ecs_->assignComponent<CRenderOffset>(newMoveableTile);
                ecs_->assignComponent<TIsReachable>(newMoveableTile);
                ecs_->assignComponent<TRenderedAsMapOverlay>(newMoveableTile);
            }
        }
    }

    SShowReachableTiles(snd::ECS* ecs)
        : snd::System<TIsSelected, CPosition, CRangeMovement>(ecs)
    {
    }
};

class SRemoveReachableTiles
    : public snd::System<TIsReachable>
{
public:
    void action(snd::EntityId entityId)
    {
        if (ecs_->getAllEntitiesWith<TIsSelected>()->size())
            return;

        if (!ecs_->getAllEntitiesWith<TIsReachable>()->size())
            return;

        tilesShown = false;

        // Action
        ecs_->removeEntity(entityId);
    }
    SRemoveReachableTiles(snd::ECS* ecs)
        : snd::System<TIsReachable>(ecs)
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
