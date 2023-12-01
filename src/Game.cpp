#include "Scenes.h"
#include <raylib.h>

Game::Game()
{
}

void Game::Initialize()
{
}

void Game::Update()
{
    ProcessInput();
    UpdateState();

    BeginDrawing();
    ClearBackground(BLACK);

    RenderOutput();

    EndDrawing();
}

void Game::ProcessInput()
{
}

void Game::UpdateState()
{
}

void Game::RenderOutput()
{
}
