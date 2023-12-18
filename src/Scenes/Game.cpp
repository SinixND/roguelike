#include "Game.h"
#include "ComponentManager.h"
#include "ECS.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Range.h"
#include "ECS/Components/Sprite.h"
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

        // Define entities
        player = ECS::createEntity();

        // Assign components
        ECS::assignComponent(player, Position{2, 3});
        ECS::assignComponent(player, Range{1.0f});
        ECS::assignComponent(player, Sprite{textureManager->retrieveTexture(PLAYER)});

        // Test retrieve component
        std::cout << "Position x: " << ECS::retrieveComponent<Position>(player).x << "\n";
        std::cout << "Range r: " << ECS::retrieveComponent<Range>(player).range << "\n";

        // Test retrieve entityMask
        auto entityMask{ECS::retrieveEntityMask(player)};
        std::cout << "Size of entityMask is: " << sizeof(entityMask) << "\n";
        std::cout << "EntityId has following components:\n";
        for (auto& component : entityMask)
        {
            std::cout << component << "\n";
        }

        // Test retrieve componentMask
        auto componentMask{ECS::retrieveComponentMask<Position>()};
        std::cout << "Component is assigned to following entities:\n";
        for (auto& entity : componentMask)
        {
            std::cout << entity << "\n";
        }

        // Test remove component
        ECS::removeComponent<Position>(player);
        ECS::removeComponent<Range>(player);
        ECS::removeComponent<Sprite>(player);
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput(){};

    void GameScene::deinitialize(){};

}
