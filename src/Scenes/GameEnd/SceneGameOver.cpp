#include "SceneGameOver.h"

#include "Colors.h"
#include "GameFont.h"
#include "Scenes.h"
#include "raylibEx.h"
#include <raylib.h>

namespace SceneGameOverModule
{
    SceneGameOver const& init( SceneGameOver& scene )
    {
        int renderWidth{ GetRenderWidth() };
        int renderHeight{ GetRenderHeight() };

        scene.panel.setRectangle(
            RectangleEx{
                0.25f * renderWidth,
                0.25f * renderHeight,
                0.5f * static_cast<float>( renderWidth ),
                0.5f * static_cast<float>( renderHeight )
            }
        );

        return scene;
    }

    SceneGameOver const& update( SceneGameOver& scene )
    {
        BeginDrawing();
        ClearBackground( Colors::bg );

        char const* line1{ "You died." };
        char const* line2{ ">> GAME OVER <<" };
#if !defined( EMSCRIPTEN )
        char const* line3{ "Press [F4] to quit." };
#else
        char const* line3{ "Reload to restart" };
#endif

        DrawTextExCentered(
            GameFont::font(),
            line1,
            scene.panel.inner(),
            -2 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            line2,
            scene.panel.inner(),
            0,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            line3,
            scene.panel.inner(),
            2 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        ScenesModule::drawWindowBorder();

        EndDrawing();

        return scene;
    }
}
