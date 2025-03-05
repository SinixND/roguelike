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

    [[nodiscard]]
    SceneGameOver const& update( SceneGameOver& scene )
    {
        BeginDrawing();
        ClearBackground( Colors::bg );

        char const* line1{ ">> GAME OVER <<" };
        char const* line2{ "You died." };
#if !defined( EMSCRIPTEN )
        char const* line3{ "([Interact] to restart)" };
#else
        char const* line3{ "(Reload to restart)" };
#endif

        DrawRectangleLinesEx(
            scene.panel.outer().rectangle(),
            1,
            Colors::border
        );

        DrawTextExCentered(
            GameFont::font(),
            line1,
            scene.panel.inner(),
            -3 * GameFont::fontSize,
            3 * GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            line2,
            scene.panel.inner(),
            0,
            2 * GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            line3,
            scene.panel.inner(),
            2 * GameFont::fontSize,
            1 * GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        //* Draw simple frame
        ScenesModule::drawWindowBorder();

        EndDrawing();

        return scene;
    }
}
