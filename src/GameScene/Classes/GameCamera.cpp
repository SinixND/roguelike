#include "GameCamera.h"
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
    snx::Publisher::publish(Event::cameraChanged);
}

void GameCamera::setOffset(Vector2 const& offset)
{
    camera_.offset = offset;
    snx::Publisher::publish(Event::cameraChanged);
}

void GameCamera::setTarget(Vector2 const& target)
{
    camera_.target = target;
    snx::Publisher::publish(Event::cameraChanged);
}

void GameCamera::setZoom(float zoom)
{
    camera_.zoom = zoom;
    snx::Publisher::publish(Event::cameraChanged);
}
