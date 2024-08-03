#include "GameCamera.h"

#include "GameScene.h"
#include "Event.h"
#include "PublisherStatic.h"

#include <raylib.h>

void GameCamera::init(Vector2 const& offset)
{
    camera_ = Camera2D{
        offset,
        Vector2{0, 0},
        0,
        1};
    GameScene::eventDispatcher.publish(Event::cameraChanged);
}

void GameCamera::setOffset(Vector2 const& offset)
{
    camera_.offset = offset;
    GameScene::eventDispatcher.publish(Event::cameraChanged);
}

void GameCamera::setTarget(Vector2 const& target)
{
    camera_.target = target;
    GameScene::eventDispatcher.publish(Event::cameraChanged);
}

void GameCamera::setZoom(float zoom)
{
    camera_.zoom = zoom;
    GameScene::eventDispatcher.publish(Event::cameraChanged);
}
