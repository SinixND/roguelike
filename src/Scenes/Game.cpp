#include "Game.h"

#include "CONFIGS.h"
#include "Components.h"
#include "ECS.h"
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
    auto renderSystem{ecs.registerSystem<RenderSystem>()};
    auto controlSystem{ecs.registerSystem<MouseCursorSystem>()};
    //=================================

    void GameScene::initialize()
    {
        // Assign components
        //=============================
        ecs.assignComponent<PositionComponent>(player);
        ecs.assignComponent<TextureComponent>(player, TEXTURE_MANAGER::getInstance()->retrieveTexture(PLAYER));
        ecs.assignComponent<RotationComponent>(player);
        ecs.assignComponent<TransformComponent>(player, ecs.retrieveComponent<PositionComponent>(player)->position_);

        ecs.assignComponent<PositionComponent>(cursor);
        ecs.assignComponent<TextureComponent>(cursor, TEXTURE_MANAGER::getInstance()->retrieveTexture(CURSOR));
        ecs.assignComponent<RotationComponent>(cursor);
        ecs.assignComponent<MouseControlFlag>(cursor);
        ecs.assignComponent<TransformComponent>(cursor, ecs.retrieveComponent<PositionComponent>(player)->position_);
        //=============================
    };

    void GameScene::processInput()
    {
        // Execute systems
        //=============================
        controlSystem->execute();
        //=============================
    };

    void GameScene::updateState(){};

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
        ecs.removeComponent<PositionComponent>(player);
        ecs.removeComponent<TextureComponent>(player);
        //=============================
    };
}
