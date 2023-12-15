#include "Game.h"
// #include "ECS.h"
#include "AttributeManager.h"
#include "ECS/Components/PositionAttribute.h"
#include "Entity.h"
#include "EntityManager.h"
#include <iostream>
#include <memory> // make_unique pointer

namespace snd
{
    // Instantiate managers
    auto entityManager{std::make_unique<EntityManager>()};
    auto positionManager{std::make_unique<AttributeManager<Position>>()};

    // Declare entities
    Entity player;

    void GameScene::initialize()
    {
        // Define entities
        player = entityManager->create();

        // Assign attributes
        positionManager->assign(player, Position{2, 3});
        positionManager->remove(player);
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput(){};

    void GameScene::deinitialize(){};

}
