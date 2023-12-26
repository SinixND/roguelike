#include "Game.h"

#include "Component.h"
#include "ComponentManager.h"
#include "Components/Position.h"
#include "Components/Texture.h"
#include "ECS.h"
#include "EntityId.h"
#include "IdManager.h"
#include "TextureManager.h"
#include <iostream>
#include <memory> // make_unique pointer
#include <raylib.h>

namespace snd
{
    // Declare entities
    EntityId player;

    void GameScene::initialize()
    {
        // Initialize ECS
        ECS::init();
        TextureManager* textureManager{TextureManager::getInstance()};
        ECS::registerComponent<Position>();
        ECS::registerComponent<Texture>();

        // Define entities
        player = ECS::createEntity();

        // Test remove component
        std::cout << "STEP: Test remove Position\n";
        ECS::removeComponent<Position>(player);

        // Assign components
        std::cout << "STEP: Assign Position\n";
        ECS::assignComponent<Position>(player, Position{2, 3});
        std::cout << "STEP: Assign Texture\n";
        ECS::assignComponent<Texture>(player, Texture{textureManager->retrieveTexture(PLAYER)});

        // Test retrieve component
        std::cout << "STEP: Test retrieve Position\n";
        std::cout << "Position x: " << ECS::retrieveComponent<Position>(player)->x_ << "\n";

        // Test remove component
        std::cout << "STEP: Test remove Position\n";
        ECS::removeComponent<Position>(player);
        std::cout << "STEP: Test remove Texture\n";
        ECS::removeComponent<Texture>(player);
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput(){};

    void GameScene::deinitialize(){};

}
