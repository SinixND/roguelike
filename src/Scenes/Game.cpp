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
    std::unique_ptr<ECS> ecs{
        std::make_unique<ECS>()};
    //=================================

    // Initialize entities
    //=================================
    EntityId player{
        ecs->createEntity()};
    //=================================

    // Initialize systems
    //=================================
    std::shared_ptr<RenderSystem> renderSystem{
        ecs->registerSystem<RenderSystem>()};
    //=================================

    void GameScene::initialize()
    {

        // Initialize texture manager
        //=================================
        TextureManager* textureManager{TextureManager::getInstance()};
        //=================================

        // Test remove component
        //=============================
        std::cout << "STEP: Test remove Position\n";
        ecs->removeComponent<PositionComponent>(player);
        //=============================

        // Assign components
        //=============================
        std::cout << "STEP: Assign Position\n";
        ecs->assignComponent<PositionComponent>(player, PositionComponent{2, 3});
        std::cout << "STEP: Assign Texture\n";
        ecs->assignComponent<TextureComponent>(player, TextureComponent{textureManager->retrieveTexture(PLAYER)});
        //=============================

        // Retrieve component
        //=============================
        std::cout << "STEP: Retrieve Position\n";
        std::cout << "Position x: " << ecs->retrieveComponent<PositionComponent>(player)->x_ << "\n";
        //=============================

        // Remove component
        //=============================
        std::cout << "STEP: Remove Position\n";
        ecs->removeComponent<PositionComponent>(player);
        std::cout << "STEP: Remove Texture\n";
        ecs->removeComponent<TextureComponent>(player);
        //=============================
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput()
    {
        renderSystem->execute();

        Configs::getInstance()->closeApp();
    };

    void GameScene::deinitialize(){};
}
