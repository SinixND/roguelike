#include "GameOverScreen.h"

#include "ColorData.h"
#include "GameFont.h"
#include "WindowSystem.h"
#include "raylibEx.h"
#include <raylib.h>

void GameOverScreen::init()
{
    int renderWidth{ GetRenderWidth() };
    int renderHeight{ GetRenderHeight() };

    panel.setRectangle(
        RectangleEx{
            0.1f * renderWidth,
            0.1f * renderHeight,
            0.8f * static_cast<float>( renderWidth ),
            0.8f * static_cast<float>( renderHeight )
        }
    );
}

void GameOverScreen::update()
{
    BeginDrawing();
    ClearBackground( ColorData::BG );

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
        panel.inner(),
        -2 * GameFont::fontSize,
        GameFont::fontSize,
        0,
        LIGHTGRAY
    );

    DrawTextExCentered(
        GameFont::font(),
        line2,
        panel.inner(),
        0,
        GameFont::fontSize,
        0,
        LIGHTGRAY
    );

    DrawTextExCentered(
        GameFont::font(),
        line3,
        panel.inner(),
        2 * GameFont::fontSize,
        GameFont::fontSize,
        0,
        LIGHTGRAY
    );

    WindowSystem::drawWindowBorder();

    EndDrawing();
}
