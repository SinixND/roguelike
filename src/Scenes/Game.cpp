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
        ecs->registerComponent<Position>();
        ecs->registerComponent<Texture>();
        // ============================

        // Register Systems
        // ============================
        ecs->registerSystem<Render>();
        // ============================

        // Define entities
        // ============================
        player = ecs->createEntity();
        // ============================

        // Test remove component
        // ============================
        std::cout << "STEP: Test remove Position\n";
        ecs->removeComponent<Position>(player);
        // ============================

        // Assign components
        // ============================
        std::cout << "STEP: Assign Position\n";
        ecs->assignComponent<Position>(player, Position{2, 3});
        std::cout << "STEP: Assign Texture\n";
        ecs->assignComponent<Texture>(player, Texture{textureManager->retrieveTexture(PLAYER)});
        // ============================

        // Retrieve component
        // ============================
        std::cout << "STEP: Retrieve Position\n";
        std::cout << "Position x: " << ecs->retrieveComponent<Position>(player)->x_ << "\n";
        // ============================

        // Remove component
        // ============================
        std::cout << "STEP: Remove Position\n";
        ecs->removeComponent<Position>(player);
        std::cout << "STEP: Remove Texture\n";
        ecs->removeComponent<Texture>(player);
        // ============================
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput(){};

    void GameScene::deinitialize(){};

}
