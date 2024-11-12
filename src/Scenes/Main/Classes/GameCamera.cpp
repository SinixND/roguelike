#include "GameCamera.h"

#include "EventID.h"
#include "PublisherStatic.h"
#include "UnitConversion.h"
#include "raylibEx.h"
#include <raylib.h>

void GameCamera::init(RectangleEx const& viewport, Vector2 const& heroPosition)
{
    updateViewport(viewport);

    camera_ = Camera2D{
        viewport.center(),
        heroPosition,
        0,
        0.75};

    snx::PublisherStatic::publish(EventID::CAMERA_CHANGED);
}

void GameCamera::setOffset(Vector2 const& offset)
{
    camera_.offset = offset;
    snx::PublisherStatic::publish(EventID::CAMERA_CHANGED);
}

void GameCamera::setTarget(Vector2 const& target)
{
    camera_.target = target;
    snx::PublisherStatic::publish(EventID::CAMERA_CHANGED);
}

void GameCamera::setZoom(float zoom)
{
    camera_.zoom = zoom;
    snx::PublisherStatic::publish(EventID::CAMERA_CHANGED);
}

void GameCamera::updateViewport(RectangleEx const& viewport)
{
    viewport_ = &viewport;
}

Camera2D const& GameCamera::camera() const
{
    return camera_;
}

RectangleEx const& GameCamera::viewportOnScreen() const
{
    return *viewport_;
}

RectangleExI GameCamera::viewportInTiles() const
{
    return RectangleExI{
        UnitConversion::screenToTile(
            viewport_->topLeft(),
            camera_),
        UnitConversion::screenToTile(
            viewport_->bottomRight(),
            camera_)};
}