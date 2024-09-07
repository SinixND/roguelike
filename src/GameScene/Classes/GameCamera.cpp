#include "GameCamera.h"

#include "Event.h"
#include "PublisherStatic.h"
#include "raylibEx.h"

#include <raylib.h>

void GameCamera::init(RectangleEx const& viewport, Vector2 const& heroPosition)
{
    updateViewport(viewport);

    camera_ = Camera2D{
        viewport.center(),
        heroPosition,
        0,
        1};

    snx::PublisherStatic::publish(Event::cameraChanged);
}

void GameCamera::setOffset(Vector2 const& offset)
{
    camera_.offset = offset;
    snx::PublisherStatic::publish(Event::cameraChanged);
}

void GameCamera::setTarget(Vector2 const& target)
{
    camera_.target = target;
    snx::PublisherStatic::publish(Event::cameraChanged);
}

void GameCamera::setZoom(float zoom)
{
    camera_.zoom = zoom;
    snx::PublisherStatic::publish(Event::cameraChanged);
}

void GameCamera::updateViewport(RectangleEx const& viewport)
{
    viewport_ = &viewport;
}
