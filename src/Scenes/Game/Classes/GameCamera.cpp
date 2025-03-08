#include "GameCamera.h"

#include "Convert.h"
#include "GameFont.h"
#include "TileData.h"
#include "raylibEx.h"
#include <raylib.h>

namespace GameCameraModule
{
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
            GameFont::fontSize / TileData::TILE_SIZE
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

    Camera2D const& setTarget(
        Camera2D& camera,
        Vector2 const& target
    )
    {
        camera.target = target;

        return camera;
    }
}
