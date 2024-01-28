#ifndef SADDREACHABLETILES_H_20240128171147
#define SADDREACHABLETILES_H_20240128171147

#include "System.h"

#include "CPosition.h"
#include "CRangeMovement.h"
#include "CTexture.h"
#include "CTransform.h"
#include "ECS.h"
#include "PathFinding.h"
#include "RuntimeDatabase.h"
#include "TReachable.h"
#include "TRenderedAsMapOverlay.h"
#include "TSelected.h"
#include "Utility.h"
#include <raymath.h>

class SAddReachableTiles
    : public snd::System<TSelected, CPosition, CRangeMovement>
{
public:
    void action(snd::EntityId entityId)
    {
        if (dtb::Configs::areTilesShown())
            return;

        dtb::Configs::showTiles();

        // Get components
        auto position{ecs_->getComponent<CPosition>(entityId)->getPosition()};
        auto moveRange{ecs_->getComponent<CRangeMovement>(entityId)->getMovementRange()};

        // Action
        auto reachablePositions{filterTilesReachable(position, moveRange, ecs_)};

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

                if (!isPositionInList(newTilePosition, reachablePositions))
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

    SAddReachableTiles(snd::ECS* ecs)
        : snd::System<TSelected, CPosition, CRangeMovement>(ecs)
    {
    }
};

#endif
