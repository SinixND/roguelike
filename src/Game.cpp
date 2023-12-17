#include "Game.h"
// #include "ECS.h"
#include "ComponentManager.h"
#include "ECS.h"
#include "ECS/Components/PositionComponent.h"
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
        ECS::assignComponent(player, Position{2, 3});
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput(){};

    void GameScene::deinitialize(){};

}
