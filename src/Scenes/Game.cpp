#include "Game.h"

#include "ECS.h"
#include "RuntimeDatabase.h"
#include <memory>
#include <raylibEx.h>

// Systems
//=================================
#include "SAddReachableTiles.h"
#include "SFollowMouse.h"
#include "SGenerateMap.h"
#include "SRemoveReachableTiles.h"
#include "SRenderMap.h"
#include "SRenderMapOverlay.h"
#include "SRenderObjects.h"
#include "SRenderUI.h"
#include "SSelection.h"
#include "STagUnderCursor.h"
//=================================

// Initialize ECS
//=================================
snd::ECS ECS;
//=================================

// Initialize entities
//=================================
auto cursor{ECS.createEntity()};
auto player{ECS.createEntity()};
auto tileMap{ECS.createEntity()};
//=================================

// Initialize systems
//=================================
auto controlSystem{ECS.registerSystem<SFollowMouse>()};
auto mapRenderSystem{ECS.registerSystem<SRenderMap>()};
auto mapOverlayRenderSystem{ECS.registerSystem<SRenderMapOverlay>()};
auto objectRenderSystem{ECS.registerSystem<SRenderObjects>()};
auto UIRenderSystem{ECS.registerSystem<SRenderUI>()};
auto tagUnderCursorSystem{ECS.registerSystem<STagUnderCursor>()};
auto selectionSystem{ECS.registerSystem<SSelection>()};
auto addReachableTiles{ECS.registerSystem<SAddReachableTiles>()};
auto removeReachableTiles{ECS.registerSystem<SRemoveReachableTiles>()};
auto generateMap{ECS.registerSystem<SGenerateMap>()};
//=================================

void GameScene::initialize()
{
    // Assign components
    //=============================
    // Player
    ECS.assignComponent<CPosition>(player);
    ECS.assignComponent<CTexture>(player, dtb::Textures::get(PLAYER_TEXTURE));
    ECS.assignComponent<CTransform>(player);
    ECS.assignComponent<CRangeMovement>(player, 15);
    ECS.assignComponent<TRenderedAsObject>(player);
    ECS.assignComponent<THoverable>(player);
    ECS.assignComponent<TSelectable>(player);
    //* ecs.assignComponent<TIsSolid>(player);

    // Cursor
    ECS.assignComponent<CPosition>(cursor);
    ECS.assignComponent<CTexture>(cursor, dtb::Textures::get(CURSOR_TEXTURE));
    ECS.assignComponent<CTransform>(cursor);
    ECS.assignComponent<TRenderedAsUI>(cursor);
    ECS.assignComponent<TMouseControlled>(cursor);
    ECS.assignComponent<TIsCursor>(cursor);

    // Map
    ECS.assignComponent<CTileMap>(tileMap);
    //=============================
};

void GameScene::processInput()
{
    // Toggle between mouse or key control for cursor
    //=============================
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        ECS.toggleComponent<TMouseControlled>(cursor);
        //* ecs.toggleComponent<TKeyControlled>(cursor);
    }
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
    generateMap->execute();
    addReachableTiles->execute();
    removeReachableTiles->execute();
    //=============================
};

void GameScene::renderOutput()
{
    // Execute systems
    //=============================
    mapRenderSystem->execute();
    mapOverlayRenderSystem->execute();
    objectRenderSystem->execute();
    UIRenderSystem->execute();
    //=============================
};

void GameScene::deinitialize(){};
