#include "Game.h"

#include "CONFIGS.h"
#include "Components.h"
#include "ECS.h"
#include "MapGenerator.h"
#include "Systems.h"
#include "TEXTURE_MANAGER.h"
#include "raylib.h"
#include <iostream>
#include <memory>

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
    int level{0};
    //=================================

    void GameScene::initialize()
    {
        // Assign components
        //=============================
        ecs.assignComponent<CPosition>(player);
        ecs.assignComponent<CTexture>(player, TEXTURE_MANAGER::getInstance()->retrieveTexture(PLAYER));
        ecs.assignComponent<CRotation>(player);
        ecs.assignComponent<CTransform>(player, ecs.retrieveComponent<CPosition>(player)->getPosition());
        ecs.assignComponent<FKeyControlled>(player);

        ecs.assignComponent<CPosition>(cursor);
        ecs.assignComponent<CTexture>(cursor, TEXTURE_MANAGER::getInstance()->retrieveTexture(CURSOR));
        ecs.assignComponent<CRotation>(cursor);
        ecs.assignComponent<CTransform>(cursor, ecs.retrieveComponent<CPosition>(player)->getPosition());
        ecs.assignComponent<FMouseControlled>(cursor);
        //=============================

        // Get map
        //=============================
        mapGenerator.generateMap(level);
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
        //* CONFIGS::getInstance()->closeApp();
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
