#include "Game.h"

#include "RuntimeDatabase.h"
#include <iostream>
#include <raylib.h>

void GameScene::initialize(){};

void GameScene::processInput()
{
    // Toggle between mouse or key control for cursor
    //=============================
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        dtb::Globals::toggleMouseActivated();
    }
    //=============================
};

void GameScene::updateState(){};

void GameScene::renderOutput()
{
    Vector2 worldMouse{GetScreenToWorld2D(GetMousePosition(), dtb::Globals::getCamera())};
    std::cout << "Mouseposition: " << GetMouseX() << ", " << GetMouseY() << ".\n";
    std::cout << "WorldMouseposition: " << worldMouse.x << ", " << worldMouse.y << ".\n";
};

void GameScene::deinitialize(){};
