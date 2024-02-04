#ifndef STAGPATHTILES_H_20240204172511
#define STAGPATHTILES_H_20240204172511

#include "CTexture.h"
#include "System.h"

#include "CPosition.h"
#include "CRangeMovement.h"
#include "ECS.h"
#include "PathFinding.h"
#include "RuntimeDatabase.h"
#include "TPathTarget.h"
#include "TPathTile.h"
#include "TReachable.h"
#include "TSelected.h"
#include "raylibEx.h"
#include <cmath>

class STagPathTiles
    : public snd::System<TReachable>
{
public:
    void action(snd::EntityId entityId)
    {
        // Skip if nothing is selected
        if (!ecs_->getFirstEntityWith<TSelected>()) return;

        auto origin{ecs_->getFirstEntityWith<TSelected>()};
        auto originPosition{ecs_->getComponent<CPosition>(origin)->getPosition()};

        static Vector2Int pathTargetPosition{originPosition};

        // Skip if no PathTarget exists
        auto pathTarget{ecs_->getFirstEntityWith<TPathTarget>()};
        if (!pathTarget)
        {
            pathTargetPosition = {originPosition};
            dtb::State::clearFoundPath();
            dtb::State::hidePath();
        }
        else
        {

            auto currentPathTargetPosition{ecs_->getComponent<CPosition>(pathTarget)->getPosition()};
            // Check if path needs to be updated
            if (!Vector2Equals(pathTargetPosition, currentPathTargetPosition))
            {
                pathTargetPosition = currentPathTargetPosition;
                dtb::State::showPath();

                auto originMovementRange{ecs_->getComponent<CRangeMovement>(origin)->getMovementRange()};

                dtb::State::setFoundPath(findPath(originPosition, pathTargetPosition, originMovementRange, ecs_));
            }
        }

        // Get components
        auto entityPosition{ecs_->getComponent<CPosition>(entityId)->getPosition()};

        // Action
        if (isInTileList(entityPosition, dtb::State::getFoundPath()))
        {
            ecs_->assignComponent<TPathTile>(entityId);
            ecs_->assignComponent<CTexture>(entityId, dtb::Textures::get(PATH_TILE));
        }
        else
        {
            ecs_->removeComponent<TPathTile>(entityId);
            ecs_->assignComponent<CTexture>(entityId, dtb::Textures::get(REACHABLE_TILE));
        }
    }

    STagPathTiles(snd::ECS* ecs)
        : snd::System<TReachable>(ecs){};
};

#endif
