#include "GameCamera.h"

#include "Convert.h"
#include "EventId.h"
#include "PublisherStatic.h"
#include "raylibEx.h"
#include <raylib.h>

void GameCameraModule::init(
    GameCamera& gameCamera,
    RectangleEx const& viewport,
    Vector2 const& heroPosition)
{
    gameCamera.viewport = &viewport;

    gameCamera.camera = Camera2D{
        viewport.center(),
        heroPosition,
        0,
        0.75};

    snx::PublisherStatic::publish(EventId::CAMERA_CHANGED);
}

void GameCameraModule::setOffset(
    GameCamera& gameCamera,
    Vector2 const& offset)
{
    gameCamera.camera.offset = offset;
    snx::PublisherStatic::publish(EventId::CAMERA_CHANGED);
}

void GameCameraModule::setTarget(
    GameCamera& gameCamera,
    Vector2 const& target)
{
    gameCamera.camera.target = target;
    snx::PublisherStatic::publish(EventId::CAMERA_CHANGED);
}

void GameCameraModule::setZoom(
    GameCamera& gameCamera,
    float zoom)
{
    gameCamera.camera.zoom = zoom;
    snx::PublisherStatic::publish(EventId::CAMERA_CHANGED);
}

RectangleExI GameCameraModule::viewportInTiles(
    GameCamera& gameCamera)
{
    return RectangleExI{
        Convert::screenToTile(
            gameCamera.viewport->topLeft(),
            gameCamera.camera),
        Convert::screenToTile(
            gameCamera.viewport->bottomRight(),
            gameCamera.camera)};
}
