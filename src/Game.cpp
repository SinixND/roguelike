#include "Game.h"
#include "ComponentManager.h"
#include "ECS.h"
#include "ECS/Components/Position.h"
#include "ECS/Components/Range.h"
#include "Entity.h"
#include "EntityManager.h"
#include <iostream>
#include <memory> // make_unique pointer

namespace snd
{
    // Declare ECS
    std::unique_ptr<ECS> ecs;

    // Declare entities
    Entity player;

    void GameScene::initialize()
    {
        // Initialize ECS
        ECS::init();

        // Define entities
        player = ECS::createEntity();

        // Assign components
        ECS::assignComponent(player, Position{2.0f, 3.0f});
        ECS::assignComponent(player, Range{1.0f});

        // Test retrieve component
        std::cout << "Position x: " << ECS::retrieveComponent<Position>(player).x << "\n";
        std::cout << "Range r: " << ECS::retrieveComponent<Range>(player).range << "\n";

        // Test retrieve entityMask
        auto entityMask{ECS::retrieveEntityMask(player)};
        std::cout << "Entity has following components:\n";
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
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput(){};

    void GameScene::deinitialize(){};

}
