#include "Game.h"

void GameScene::initialize()
{
    cursor.renderID = RenderID::cursor;

    hero.movement.range = 5;
    hero.renderID = RenderID::hero;
}
