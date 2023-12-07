#include "Game.h"
#include "Character.h"
#include "Units.h"
#include <raylib.h>

namespace snd
{
    Game::Game()
        : player_(Vector2{0, 0}, 10, GOLD)
    {
    }

    void Game::initialize()
    {
        player_.setPosition(Vector2{GetScreenWidth() / 2.0f, GetRenderHeight() / 2.0f});
    };

    void Game::processInput()
    {
        switch (GetKeyPressed())
        {
        case KEY_LEFT:
            player_.move(Unit::left(), Unit::field());
            break;
        case KEY_RIGHT:
            player_.move(Unit::right(), Unit::field());
            break;
        case KEY_UP:
            player_.move(Unit::up(), Unit::field());
            break;
        case KEY_DOWN:
            player_.move(Unit::down(), Unit::field());
            break;
        default:
            break;
        }
    };

    void Game::updateState(){};

    void Game::renderOutput()
    {
        render(player_);
    };

    void Game::deinitialize(){};
}