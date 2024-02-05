#include "Game.h"

#include "RuntimeDatabase.h"
#include <raylib.h>

void GameScene::initialize(){};

void GameScene::processInput()
{
    // Toggle between mouse or key control for cursor
    //=============================
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        dtb::State::toggleMouseActivated();
    }
    //=============================
};

void GameScene::updateState(){};

void GameScene::renderOutput([[maybe_unused]] Camera2D& camera){};

void GameScene::deinitialize(){};
