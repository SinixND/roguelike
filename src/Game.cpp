#include "Game.h"
#include "Entity.h"
#include "EntityManager.h"
#include "PositionManager.h"
#include <iostream>
#include <memory>
#include <utility>

namespace snd
{
    // Instantiate managers
    auto entityManager{std::make_unique<EntityManager>()};
    auto positionManager{std::make_unique<PositionManager>()};

    // Declare entities
    Entity player;

    void GameScene::initialize()
    {
        // Define entities
        player = entityManager->createEntity();

        // Assign components
        positionManager->assignPosition(player, Position{2, 3});
        positionManager->removePosition(player);
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput(){};

    void GameScene::deinitialize(){};

}
