#include "Game.h"
#include "LayerID.h"
#include "RenderID.h"

void GameScene::initialize()
{
    cursor.graphic.position = &cursor.position;
    cursor.graphic.renderID = RenderID::cursor;
    cursor.graphic.layerID = LayerID::UI;

    hero.movement.range = 5;
    hero.graphic.position = &hero.position;
    hero.graphic.renderID = RenderID::hero;
    hero.graphic.layerID = LayerID::object;
}
