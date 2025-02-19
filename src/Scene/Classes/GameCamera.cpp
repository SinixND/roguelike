#include "GameCamera.h"

#include "Convert.h"
#include "EventDispatcher.h"
#include "Events.h"
#include "raylibEx.h"
#include <raylib.h>

namespace GameCameraModule
{
    [[nodiscard]]
    GameCamera const& init(
        GameCamera& gameCamera,
        RectangleEx const& viewport,
        Vector2 const& heroPosition
    )
    {
        gameCamera.viewport = &viewport;

        gameCamera.camera = Camera2D{
            viewport.center(),
            heroPosition,
            0,
            0.75
        };

        snx::EventDispatcher::notify( EventId::CAMERA_CHANGED );

        return gameCamera;
    }

    RectangleExI viewportInTiles(
        GameCamera& gameCamera
    )
    {
        return RectangleExI{
            Convert::screenToTile(
                gameCamera.viewport->topLeft(),
                gameCamera.camera
            ),
            Convert::screenToTile(
                gameCamera.viewport->bottomRight(),
                gameCamera.camera
            )
        };
    }
    [[nodiscard]]
    GameCamera const& setOffset(
        GameCamera& gameCamera,
        Vector2 const& offset
    )
    {
        gameCamera.camera.offset = offset;
        snx::EventDispatcher::notify( EventId::CAMERA_CHANGED );

        return gameCamera;
    }

    [[nodiscard]]
    GameCamera const& setTarget(
        GameCamera& gameCamera,
        Vector2 const& target
    )
    {
        gameCamera.camera.target = target;
        snx::EventDispatcher::notify( EventId::CAMERA_CHANGED );

        return gameCamera;
    }

    [[nodiscard]]
    GameCamera const& setZoom(
        GameCamera& gameCamera,
        float zoom
    )
    {
        gameCamera.camera.zoom = zoom;
        snx::EventDispatcher::notify( EventId::CAMERA_CHANGED );

        return gameCamera;
    }
}
