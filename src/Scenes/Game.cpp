#include "Game.h"

#include "CONFIGS.h"
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentOrientation.h"
#include "ComponentPosition.h"
#include "ComponentTexture.h"
#include "ECS.h"
#include "EntityId.h"
#include "IdManager.h"
#include "SystemRender.h"
#include "TEXTURE_MANAGER.h"
#include <iostream>
#include <memory>
#include <raylib.h>

namespace snd
{
    // Initialize ECS
    //=================================
    ECS ecs;
    //=================================

    // Initialize entities
    //=================================
    auto player{
        ecs.createEntity()};
    //=================================

    // Initialize systems
    //=================================
    auto renderSystem{
        ecs.registerSystem<RenderSystem>()};
    //=================================

    void GameScene::initialize()
    {
        // Assign components
        //=============================
        TEXTURE_MANAGER* textureManager{TEXTURE_MANAGER::getInstance()};
        ecs.assignComponent<TextureComponent>(player, textureManager->retrieveTexture(PLAYER));
        ecs.assignComponent<PositionComponent>(player, Vector2{150, 100});
        ecs.assignComponent<DirectionComponent>(player, RIGHT);

        //=============================
    };

    void GameScene::processInput(){};

    void GameScene::updateState()
    {
        std::cout << "STEP: UpdateState GameScene\n";

        // Retrieve component
        //=============================
        std::cout << "STEP: Retrieve Position\n";
        std::cout << "Position x: " << ecs.retrieveComponent<PositionComponent>(player)->position_.x << "\n";
        //=============================
    };

    void GameScene::renderOutput()
    {
        std::cout << "STEP: Execute renderSystem\n";

        // Execute systems
        //=============================
        renderSystem->execute();
        //=============================

        // Call app termination
        //* Configs::getInstance()->closeApp();
    };

    void GameScene::deinitialize()
    {
        std::cout << "STEP: Deinitialize GameScene\n";

        // Remove component
        //=============================
        std::cout << "STEP: Remove Position\n";
        ecs.removeComponent<PositionComponent>(player);
        std::cout << "STEP: Remove Texture\n";
        ecs.removeComponent<TextureComponent>(player);
        //=============================
    };
}
