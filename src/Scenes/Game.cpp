#include "Game.h"

#include "Component.h"
#include "ComponentManager.h"
#include "ComponentPosition.h"
#include "ComponentTexture.h"
#include "Configs.h"
#include "ECS.h"
#include "EntityId.h"
#include "IdManager.h"
#include "SystemRender.h"
#include "TextureManager.h"
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
    auto player{
        ecs.createEntity()};
    //=================================

    // Initialize systems
    //=================================
    auto renderSystem{
        ecs.registerSystem<RenderSystem>()};
    //=================================

    void GameScene::initialize()
    {
        std::cout << "STEP: Initialize GameScene\n";

        // Initialize texture manager
        //=================================
        TextureManager* textureManager{TextureManager::getInstance()};
        //=================================

        // Test remove component
        //=============================
        std::cout << "STEP: Test remove Position\n";
        ecs.removeComponent<PositionComponent>(player);
        //=============================

        // Assign components
        //=============================
        std::cout << "STEP: Assign Position\n";
        ecs.assignComponent<PositionComponent>(player, PositionComponent{2, 3});
        std::cout << "STEP: Assign Texture\n";
        ecs.assignComponent<TextureComponent>(player, TextureComponent{textureManager->retrieveTexture(PLAYER)});
        //=============================
    };

    void GameScene::processInput(){};

    void GameScene::updateState()
    {
        std::cout << "STEP: UpdateState GameScene\n";

        // Retrieve component
        //=============================
        std::cout << "STEP: Retrieve Position\n";
        std::cout << "Position x: " << ecs.retrieveComponent<PositionComponent>(player)->x_ << "\n";
        //=============================
    };

    void GameScene::renderOutput()
    {
        std::cout << "STEP: Execute renderSystem\n";

        // Execute systems
        //=============================
        renderSystem->execute();
        //=============================

        // Call app termination
        Configs::getInstance()->closeApp();
    };

    void GameScene::deinitialize()
    {
        std::cout << "STEP: Deinitialize GameScene\n";

        // Remove component
        //=============================
        std::cout << "STEP: Remove Position\n";
        ecs.removeComponent<PositionComponent>(player);
        std::cout << "STEP: Remove Texture\n";
        ecs.removeComponent<TextureComponent>(player);
        //=============================
    };
}
