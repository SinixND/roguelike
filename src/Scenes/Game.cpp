#include "Game.h"

#include "RuntimeDatabase.h"
#include "Components.h"
#include "ECS.h"
#include "EntityId.h"
#include "Map.h"
#include "MapGenerator.h"
#include "Systems.h"
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
        ecs.assignComponent<CTexture>(player, dtb::Textures::retrieve(PLAYER_TEXTURE));
        ecs.assignComponent<COrientation>(player);
        ecs.assignComponent<CTransformation>(player, ecs.retrieveComponent<CPosition>(player)->getPosition());
        ecs.assignComponent<FKeyControlled>(player);

        ecs.assignComponent<CPosition>(cursor);
        ecs.assignComponent<CTexture>(cursor, dtb::Textures::retrieve(CURSOR_TEXTURE));
        ecs.assignComponent<COrientation>(cursor);
        ecs.assignComponent<CTransformation>(cursor);
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
            ecs.assignComponent<COrientation>(tileEntity);
            ecs.assignComponent<CTransformation>(tileEntity);

            switch (*tiles->get(tilePosition))
            {

            case WALL_TILE:
                ecs.assignComponent<CTexture>(tileEntity, dtb::Textures::retrieve(WALL_TEXTURE));
                break;

            case FLOOR_TILE:
                ecs.assignComponent<CTexture>(tileEntity, dtb::Textures::retrieve(FLOOR_TEXTURE));
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
