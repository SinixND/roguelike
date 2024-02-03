#include "Game.h"

#include "ECS.h"
#include "RuntimeDatabase.h"
#include <memory>
#include <raylibEx.h>

// Systems
//=================================
#include "SAddReachableTiles.h"
#include "SControl.h"
#include "SFollowMouse.h"
#include "SGenerateMap.h"
#include "SRemoveReachableTiles.h"
#include "SRenderMap.h"
#include "SRenderMapOverlay.h"
#include "SRenderObjects.h"
#include "SRenderUI.h"
#include "SSelection.h"
#include "STagUnderCursor.h"
#include "TKeyControlled.h"
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
auto sFollowMouse{ECS.registerSystem<SFollowMouse>()};
auto sRenderMap{ECS.registerSystem<SRenderMap>()};
auto sRenderMapOverlay{ECS.registerSystem<SRenderMapOverlay>()};
auto sRenderObjects{ECS.registerSystem<SRenderObjects>()};
auto sRenderUI{ECS.registerSystem<SRenderUI>()};
auto sTagUnderCursor{ECS.registerSystem<STagUnderCursor>()};
auto sSelection{ECS.registerSystem<SSelection>()};
auto sAddReachableTiles{ECS.registerSystem<SAddReachableTiles>()};
auto sRemoveReachableTiles{ECS.registerSystem<SRemoveReachableTiles>()};
auto sGenerateMap{ECS.registerSystem<SGenerateMap>()};
auto sControl{ECS.registerSystem<SControl>()};
//=================================

void GameScene::initialize()
{
    // Assign components
    //=============================
    // Player
    ECS.assignComponent<CPosition>(player);
    ECS.assignComponent<CTexture>(player, dtb::Textures::get(PLAYER));
    ECS.assignComponent<CTransform>(player);
    ECS.assignComponent<CRangeMovement>(player, 5);
    ECS.assignComponent<TRenderedAsObject>(player);
    ECS.assignComponent<THoverable>(player);
    ECS.assignComponent<TSelectable>(player);

    // Cursor
    ECS.assignComponent<CPosition>(cursor);
    ECS.assignComponent<CTexture>(cursor, dtb::Textures::get(CURSOR));
    ECS.assignComponent<CTransform>(cursor);
    ECS.assignComponent<TRenderedAsUI>(cursor);
    ECS.assignComponent<TIsCursor>(cursor);
    ECS.assignComponent<TMouseControlled>(cursor);

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
        dtb::Configs::toggleMouseActivated();
        ECS.toggleComponent<TMouseControlled>(cursor);
        ECS.toggleComponent<TKeyControlled>(cursor);
    }
    //=============================

    // Execute systems
    //=============================
    sTagUnderCursor->execute();
    sSelection->execute();
    sFollowMouse->execute();
    sControl->execute();
    //=============================
};

void GameScene::updateState()
{
    // Execute systems
    //=============================
    sGenerateMap->execute();
    sAddReachableTiles->execute();
    sRemoveReachableTiles->execute();
    //=============================
};

void GameScene::renderOutput()
{
    // Execute systems
    //=============================
    sRenderMap->execute();
    sRenderMapOverlay->execute();
    sRenderObjects->execute();
    sRenderUI->execute();
    //=============================
};

void GameScene::deinitialize(){};
