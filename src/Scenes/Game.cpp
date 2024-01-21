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
    auto mapOverlayRenderSystem{ecs.registerSystem<SRenderMapOverlay>()};
    auto objectRenderSystem{ecs.registerSystem<SRenderObjects>()};
    auto UIRenderSystem{ecs.registerSystem<SRenderUI>()};
    auto tagUnderCursorSystem{ecs.registerSystem<STagUnderCursor>()};
    auto selectionSystem{ecs.registerSystem<SSelection>()};
    auto rangesRenderSystem{ecs.registerSystem<SRenderMapOverlay>()};
    auto showReachableTiles{ecs.registerSystem<SShowReachableTiles>()};
    auto removeReachableTiles{ecs.registerSystem<SRemoveReachableTiles>()};
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
        // Player
        ecs.assignComponent<CPosition>(player);
        ecs.assignComponent<CTexture>(player, dtb::Textures::get(PLAYER_TEXTURE));
        ecs.assignComponent<CRenderOffset>(player);
        ecs.assignComponent<CRangeMovement>(player, 5);
        ecs.assignComponent<TRenderedAsObject>(player);
        ecs.assignComponent<TIsHoverable>(player);
        ecs.assignComponent<TIsSelectable>(player);

        // Cursor
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
                // ecs.assignComponent<TRigid>(tileEntity);
                break;

            case FLOOR_TILE:
                ecs.assignComponent<CTexture>(tileEntity, dtb::Textures::get(FLOOR_TEXTURE));
                // ecs.assignComponent<TIsPassableTile>(tileEntity);
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

    void GameScene::updateState()
    {
        // Execute systems
        //=============================
        showReachableTiles->execute();
        removeReachableTiles->execute();
        //=============================
    };

    void GameScene::renderOutput()
    {
        // Execute systems
        //=============================
        mapRenderSystem->execute();
        mapOverlayRenderSystem->execute();
        rangesRenderSystem->execute();
        objectRenderSystem->execute();
        UIRenderSystem->execute();
        //=============================
    };

    void GameScene::deinitialize(){};
}
