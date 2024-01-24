#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "System.h"

class SFollowMouse
    : public snd::System<CPosition, CTransform, TMouseControlled>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};

        const auto& transform{ecs_->getComponent<CTransform>(entityId)->getTransform()};

        // Action
        position = Vector2Add(convertToTile(GetMousePosition()), transform);
    }

    SFollowMouse(snd::ECS* ecs)
        : snd::System<CPosition, CTransform, TMouseControlled>(ecs)
    {
    }
};

class SGenerateMap
    : public snd::System<TIsMap>
{
public:
    void action(snd::EntityId entityId)
    {
        if (currentLevel_ == dtb::Configs::getCurrentLevel())
            return;

        ++currentLevel_;

        auto* tileMap{ecs_->getComponent<CTileMap>(entityId)->getTileMap()};

        if (currentLevel_ == 0)
        {
            setStartRoom(tileMap);
        }
    }

    SGenerateMap(snd::ECS* ecs)
        : snd::System<TIsMap>(ecs)
    {
    }

private:
    static inline int currentLevel_{-1};
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
    : public snd::System<CTexture, CPosition, CTransform, TRenderedAsMap>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->getComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->getComponent<CTransform>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderMap(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CTransform, TRenderedAsMap>(ecs){};
};

class SRenderMapOverlay
    : public snd::System<CTexture, CPosition, CTransform, TRenderedAsMapOverlay>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->getComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->getComponent<CTransform>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderMapOverlay(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CTransform, TRenderedAsMapOverlay>(ecs){};
};

class SRenderObjects
    : public snd::System<CTexture, CPosition, CTransform, TRenderedAsObject>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->getComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->getComponent<CTransform>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderObjects(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CTransform, TRenderedAsObject>(ecs){};
};

class SRenderUI
    : public snd::System<CTexture, CPosition, CTransform, TRenderedAsUI>
{
public:
    void action(snd::EntityId entityId)
    {
        // Get components
        const auto* texture{ecs_->getComponent<CTexture>(entityId)->getTexture()};
        const auto& position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        const auto& transform{ecs_->getComponent<CTransform>(entityId)->getTransform()};

        renderAction(texture, position, transform);
    }

    SRenderUI(snd::ECS* ecs)
        : snd::System<CTexture, CPosition, CTransform, TRenderedAsUI>(ecs){};
};

class STagUnderCursor
    : public snd::System<CPosition, THoverable>
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
        : snd::System<CPosition, THoverable>(ecs){};
};

class SSelection
    : public snd::System<TIsUnderCursor, TSelectable>
{
public:
    void action(snd::EntityId entityId)
    {
        // Action
        if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ecs_->toggleComponent<TSelected>(entityId);
        }
    }
    SSelection(snd::ECS* ecs)
        : snd::System<TIsUnderCursor, TSelectable>(ecs)
    {
    }
};

static bool tilesShown{false};

class SShowReachableTiles
    : public snd::System<TSelected, CPosition, CRangeMovement>
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

                auto path{findPath(position, newTilePosition, moveRange, ecs_)};

                if (path.size() < 2)
                    continue;

                auto newMoveableTile{ecs_->createEntity()};

                ecs_->assignComponent<CPosition>(newMoveableTile, newTilePosition);
                ecs_->assignComponent<CTexture>(newMoveableTile, dtb::Textures::get(REACHABLE_TILE));
                ecs_->assignComponent<CTransform>(newMoveableTile);
                ecs_->assignComponent<TReachable>(newMoveableTile);
                ecs_->assignComponent<TRenderedAsMapOverlay>(newMoveableTile);
            }
        }
    }

    SShowReachableTiles(snd::ECS* ecs)
        : snd::System<TSelected, CPosition, CRangeMovement>(ecs)
    {
    }
};

class SRemoveReachableTiles
    : public snd::System<TReachable>
{
public:
    void action(snd::EntityId entityId)
    {
        if (ecs_->getAllEntitiesWith<TSelected>()->size())
            return;

        if (!ecs_->getAllEntitiesWith<TReachable>()->size())
            return;

        tilesShown = false;

        // Action
        ecs_->removeEntity(entityId);
    }
    SRemoveReachableTiles(snd::ECS* ecs)
        : snd::System<TReachable>(ecs)
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
