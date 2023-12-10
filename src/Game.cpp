#include "Game.h"
#include "Entity.h"
#include "EntityManager.h"

namespace snd
{
    // Instantiate managers
    EntityManager* entityManager{EntityManager::getInstance()};

    // Declare entities
    Entity player;

    // Declare components

    void GameScene::initialize()
    {
        // Declare entities
        player = entityManager->createEntity();

        // Declare components

        // Add components to entities
    };

    void GameScene::processInput(){};

    void GameScene::updateState(){};

    void GameScene::renderOutput(){};

    void GameScene::deinitialize(){};

}