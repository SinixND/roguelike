#include "Game.h"

#include "Component.h"
#include "ComponentManager.h"
#include "ComponentPosition.h"
#include "ComponentTexture.h"
#include "ECS.h"
#include "EntityId.h"
#include "IdManager.h"
#include "SystemRender.h"
#include "TextureManager.h"
#include <iostream>
#include <memory> // make_unique pointer
#include <raylib.h>

namespace snd
{
    // Initialize ECS
    // ================================
    std::unique_ptr<ECS> ecs{std::make_unique<ECS>()};
    // ================================

    // Declare entities
    // ================================
    EntityId player;
    // ================================

    void GameScene::initialize()
    {
        TextureManager* textureManager{TextureManager::getInstance()};

        // Register component types
        // ============================
        ecs->registerComponent<PositionComponent>();
        ecs->registerComponent<TextureComponent>();
        // ============================

        // Register Systems
        // ============================
        ecs->registerSystem<RenderSystem>();
        // ============================

        // Define entities
        // ============================
        player = ecs->createEntity();
        // ============================

        // Test remove component
        // ============================
        std::cout << "STEP: Test remove Position\n";
        ecs->removeComponent<PositionComponent>(player);
        // ============================

        // Assign components
        // ============================
        std::cout << "STEP: Assign Position\n";
        ecs->assignComponent<PositionComponent>(player, PositionComponent{2, 3});
        std::cout << "STEP: Assign Texture\n";
        ecs->assignComponent<TextureComponent>(player, TextureComponent{textureManager->retrieveTexture(PLAYER)});
        // ============================

        // Retrieve component
        // ============================
        std::cout << "STEP: Retrieve Position\n";
        std::cout << "Position x: " << ecs->retrieveComponent<PositionComponent>(player)->x_ << "\n";
        // ============================

        // Remove component
        // ============================
        std::cout << "STEP: Remove Position\n";
        ecs->removeComponent<PositionComponent>(player);
        std::cout << "STEP: Remove Texture\n";
        ecs->removeComponent<TextureComponent>(player);
        // ============================
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput(){};

    void GameScene::deinitialize(){};

}
