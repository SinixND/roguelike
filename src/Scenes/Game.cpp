#include "Game.h"

#include "CONFIGS.h"
#include "Components.h"
#include "ECS.h"
#include "EntityId.h"
#include "Map.h"
#include "MapGenerator.h"
#include "Systems.h"
#include "TEXTURE_MANAGER.h"
#include <iostream>
#include <memory>
#include <raylib.h>

namespace snd
{
    // Initialize ECS
    //=================================
    ECS ecs;
    //=================================

    // Initialize entities
    //=================================
    auto player{ecs.createEntity()};
    auto cursor{ecs.createEntity()};
    //=================================

    // Initialize systems
    //=================================
    auto renderSystem{ecs.registerSystem<SRender>()};
    auto mouseControlSystem{ecs.registerSystem<SMouseControl>()};
    auto rotationSystem{ecs.registerSystem<SRotation>()};
    auto movementSystem{ecs.registerSystem<SMovement>()};
    //=================================

    // Game variables
    //=================================
    MapGenerator mapGenerator;
    Map map;
    int level{0};
    //=================================

    void GameScene::initialize()
    {
        // Assign components
        //=============================
        ecs.assignComponent<CPosition>(player);
        ecs.assignComponent<CTexture>(player, TEXTURE_MANAGER::get().retrieveTexture(PLAYER_TEXTURE));
        ecs.assignComponent<CRotation>(player);
        ecs.assignComponent<CTransformation>(player, ecs.retrieveComponent<CPosition>(player)->getPosition());
        ecs.assignComponent<FKeyControlled>(player);

        ecs.assignComponent<CPosition>(cursor);
        ecs.assignComponent<CTexture>(cursor, TEXTURE_MANAGER::get().retrieveTexture(CURSOR_TEXTURE));
        ecs.assignComponent<CRotation>(cursor);
        ecs.assignComponent<CTransformation>(cursor, ecs.retrieveComponent<CPosition>(player)->getPosition());
        ecs.assignComponent<FMouseControlled>(cursor);
        //=============================

        // Create map
        //=============================
        map = mapGenerator.generateMap(level);
        auto tiles{map.getTiles()};

        for (auto tilePosition : *tiles->getAllKeys())
        {
            EntityId tileEntity{map.addEntity(ecs.createEntity())};

            ecs.assignComponent<CPosition>(tileEntity, tilePosition.first, tilePosition.second);
            ecs.assignComponent<CRotation>(tileEntity);
            ecs.assignComponent<CTransformation>(tileEntity, ecs.retrieveComponent<CPosition>(player)->getPosition());

            switch (*tiles->get(tilePosition))
            {

            case WALL_TILE:
                ecs.assignComponent<CTexture>(tileEntity, TEXTURE_MANAGER::get().retrieveTexture(WALL_TEXTURE));
                break;

            case FLOOR_TILE:
                ecs.assignComponent<CTexture>(tileEntity, TEXTURE_MANAGER::get().retrieveTexture(FLOOR_TEXTURE));
                break;

            default:
                break;
            }
        }
        //=============================
    };

    void GameScene::processInput()
    {
        // Execute systems
        //=============================
        mouseControlSystem->execute();
        rotationSystem->execute();
        movementSystem->execute();
        //=============================
    };

    void GameScene::updateState(){
        // Execute systems
        //=============================
        //=============================
    };

    void GameScene::renderOutput()
    {
        // Execute systems
        //=============================
        renderSystem->execute();
        //=============================

        // Call app termination
        //* CONFIGS::get().closeApp();
    };

    void GameScene::deinitialize()
    {
        // Remove component
        //=============================
        ecs.removeComponent<CPosition>(player);
        ecs.removeComponent<CTexture>(player);
        //=============================
    };
}
