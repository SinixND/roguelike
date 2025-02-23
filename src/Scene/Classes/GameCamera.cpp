#include "GameCamera.h"

#include "Convert.h"
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
        gameCamera.camera = Camera2D{
            viewport.center(),
            heroPosition,
            0,
            0.75
        };

        gameCamera.viewport = &viewport;

        return gameCamera;
    }

    RectangleExI viewportInTiles( GameCamera const& gameCamera )
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
    Camera2D const& setOffset(
        Camera2D& camera,
        Vector2 const& offset
    )
    {
        camera.offset = offset;

        return camera;
    }

    [[nodiscard]]
    Camera2D const& setTarget(
        Camera2D& camera,
        Vector2 const& target
    )
    {
        camera.target = target;

        return camera;
    }

    [[nodiscard]]
    Camera2D const& setZoom(
        Camera2D& camera,
        float zoom
    )
    {
        camera.zoom = zoom;

        return camera;
    }
}
