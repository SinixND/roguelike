#include "Game.h"

#include "ECS.h"
#include "RuntimeDatabase.h"
#include <memory>
#include <raylibEx.h>

// Systems
//=================================
#include "SAddReachableTiles.h"
#include "SControl.h"
#include "SDeSelect.h"
#include "SFollowMouse.h"
#include "SGenerateMap.h"
#include "SRemoveReachableTiles.h"
#include "SRenderMap.h"
#include "SRenderMapOverlay.h"
#include "SRenderObjects.h"
#include "SRenderUI.h"
#include "SSelect.h"
#include "STagPathTarget.h"
#include "STagPathTiles.h"
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
auto sAddReachableTiles{ECS.registerSystem<SAddReachableTiles>()};
auto sControl{ECS.registerSystem<SControl>()};
auto sDeSelect{ECS.registerSystem<SDeSelect>()};
auto sFollowMouse{ECS.registerSystem<SFollowMouse>()};
auto sGenerateMap{ECS.registerSystem<SGenerateMap>()};
auto sRemoveReachableTiles{ECS.registerSystem<SRemoveReachableTiles>()};
auto sRenderMap{ECS.registerSystem<SRenderMap>()};
auto sRenderMapOverlay{ECS.registerSystem<SRenderMapOverlay>()};
auto sRenderObjects{ECS.registerSystem<SRenderObjects>()};
auto sRenderUI{ECS.registerSystem<SRenderUI>()};
auto sSelect{ECS.registerSystem<SSelect>()};
auto sTagPathTarget{ECS.registerSystem<STagPathTarget>()};
auto sTagPathTiles{ECS.registerSystem<STagPathTiles>()};
auto sTagUnderCursor{ECS.registerSystem<STagUnderCursor>()};
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
        dtb::State::toggleMouseActivated();
        ECS.toggleComponent<TMouseControlled>(cursor);
        ECS.toggleComponent<TKeyControlled>(cursor);
    }
    //=============================

    // Execute systems
    //=============================
    sTagUnderCursor->execute();
    sSelect->execute();
    sDeSelect->execute();
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
    sTagPathTarget->execute();
    sTagPathTiles->execute();
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
