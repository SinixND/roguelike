#include "Game.h"
#include "Entity.h"
#include "EntityManager.h"
#include <iostream>
#include <memory>
#include <utility>

namespace snd
{
    // Instantiate managers
    auto entityManager{std::make_unique<EntityManager>()};

    // Declare entities
    Entity player;

    void GameScene::initialize()
    {
        // Define entities
        player = entityManager->createEntity();

        // Assign components to entities
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput(){};

    void GameScene::deinitialize(){};

}