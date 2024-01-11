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
    auto controlSystem{ecs.registerSystem<ControlSystem>()};
    //=================================

    void GameScene::initialize()
    {
        TEXTURE_MANAGER* textureManager{TEXTURE_MANAGER::getInstance()};

        // Assign components
        //=============================
        ecs.assignComponent<PositionComponent>(player, Vector2{150, 100});
        ecs.assignComponent<TextureComponent>(player, textureManager->retrieveTexture(PLAYER));
        ecs.assignComponent<DirectionComponent>(player, RIGHT);

        ecs.assignComponent<PositionComponent>(cursor);
        ecs.assignComponent<TextureComponent>(cursor, textureManager->retrieveTexture(CURSOR));
        ecs.assignComponent<DirectionComponent>(cursor);
        ecs.assignComponent<ControlFlag>(cursor);
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
