#include "Game.h"

#include "Cursor.h"
#include "RuntimeDatabase.h"
#include "Services/Renderer.h"
#include <iostream>
#include <raylib.h>

Renderer renderer{};
Cursor cursor{};

void GameScene::initialize()
{
    cursor.position.updateFromMousePosition();
};

void GameScene::processInput()
{
    cursor.position.updateFromMousePosition();

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
    std::cout << "Cursorposition: " << cursor.position.x << ", " << cursor.position.y << ".\n\n";

    renderer.render(cursor.renderId, cursor.position.x, cursor.position.y);
};

void GameScene::deinitialize(){};
