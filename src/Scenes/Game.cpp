#include "Game.h"

#include "Components.h"
#include "ECS.h"
#include "EntityId.h"
#include "Map.h"
#include "MapGenerator.h"
#include "RuntimeDatabase.h"
#include "Systems.h"
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
    auto cursor{ecs.createEntity()};
    auto player{ecs.createEntity()};
    //=================================

    // Initialize systems
    //=================================
    auto controlSystem{ecs.registerSystem<SFollowMouse>()};
    auto mapRenderSystem{ecs.registerSystem<SRenderMap>()};
    auto objectRenderSystem{ecs.registerSystem<SRenderObjects>()};
    auto UIRenderSystem{ecs.registerSystem<SRenderUI>()};
    auto tagUnderCursorSystem{ecs.registerSystem<STagUnderCursor>()};
    auto selectionSystem{ecs.registerSystem<SSelection>()};
    //=================================

    // Game variables
    //=================================
    MapGenerator mapGenerator;
    Map map;
    int level{0};
    //=================================

    void GameScene::initialize()
    {
        // Assign components
        //=============================
        ecs.assignComponent<CPosition>(player);
        ecs.assignComponent<CTexture>(player, dtb::Textures::get(PLAYER_TEXTURE));
        ecs.assignComponent<CRenderOffset>(player, ecs.getComponent<CPosition>(player)->getPosition());
        ecs.assignComponent<TRenderedAsObject>(player);
        ecs.assignComponent<TIsHoverable>(player);
        ecs.assignComponent<TIsSelectable>(player);

        ecs.assignComponent<CPosition>(cursor);
        ecs.assignComponent<CTexture>(cursor, dtb::Textures::get(CURSOR_TEXTURE));
        ecs.assignComponent<CRenderOffset>(cursor);
        ecs.assignComponent<TRenderedAsUI>(cursor);
        ecs.assignComponent<TMouseControlled>(cursor);
        ecs.assignComponent<TIsCursor>(cursor);
        //=============================

        // Create map
        //=============================
        map = mapGenerator.generateMap(level);
        auto tiles{map.getTiles()};

        for (auto tilePosition : *tiles->getAllKeys())
        {
            EntityId tileEntity{map.addEntity(ecs.createEntity())};

            ecs.assignComponent<CPosition>(tileEntity, tilePosition.first, tilePosition.second);
            ecs.assignComponent<CDirection>(tileEntity);
            ecs.assignComponent<CRenderOffset>(tileEntity);
            ecs.assignComponent<TRenderedAsMap>(tileEntity);

            switch (*tiles->get(tilePosition))
            {

            case WALL_TILE:
                ecs.assignComponent<CTexture>(tileEntity, dtb::Textures::get(WALL_TEXTURE));
                break;

            case FLOOR_TILE:
                ecs.assignComponent<CTexture>(tileEntity, dtb::Textures::get(FLOOR_TEXTURE));
                break;

            default:
                break;
            }
        }
        //=============================
    };

    void GameScene::processInput()
    {
        // Toggle between mouse or key control for cursor
        //=============================
        //* if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        //* {
        //* ecs.toggleComponent<TMouseControlled>(cursor);
        //* ecs.toggleComponent<TKeyControlled>(cursor);
        //* }
        //=============================

        // Execute systems
        //=============================
        tagUnderCursorSystem->execute();
        selectionSystem->execute();
        controlSystem->execute();
        //=============================
    };

    void GameScene::updateState(){
        // Execute systems
        //=============================
        //=============================
    };

    void GameScene::renderOutput()
    {
        // Execute systems
        //=============================
        mapRenderSystem->execute();
        objectRenderSystem->execute();
        UIRenderSystem->execute();
        //=============================
    };

    void GameScene::deinitialize(){};
}
