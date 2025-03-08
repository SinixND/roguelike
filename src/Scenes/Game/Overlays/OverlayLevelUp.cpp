#include "OverlayLevelUp.h"

#include "Colors.h"
#include "GameFont.h"
#include "raylibEx.h"
#include <raylib.h>

namespace OverlayLevelUpModule
{
    OverlayLevelUp const& init( OverlayLevelUp& overlay )
    {
        int renderWidth{ GetRenderWidth() };
        int renderHeight{ GetRenderHeight() };

        overlay.panel.setOuter(
            RectangleEx{
                0.25f * renderWidth,
                0.25f * renderHeight,
                0.5f * static_cast<float>( renderWidth ),
                0.5f * static_cast<float>( renderHeight )
            },
            0,
            0.5f * GameFont::fontSize
        );

        return overlay;
    }

    void update( OverlayLevelUp const& overlay )
    {
        DrawRectangleRec(
            overlay.panel.box().rectangle(),
            Colors::BG
        );

        char const* line1{ "LevelUp!" };
        char const* line2{ "1 Attribute point to spend." };
        char const* line3{ "Choose attribute:" };
        char const* line4{ "[V]itality\n[A]gility" };

        DrawTextExCentered(
            GameFont::font(),
            line1,
            overlay.panel.inner(),
            -4 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            line2,
            overlay.panel.inner(),
            -2 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            line3,
            overlay.panel.inner(),
            -1 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawTextExCentered(
            GameFont::font(),
            line4,
            overlay.panel.inner(),
            1 * GameFont::fontSize,
            GameFont::fontSize,
            0,
            LIGHTGRAY
        );

        DrawRectangleLinesEx(
            overlay.panel.outer().rectangle(),
            1,
            Colors::BORDER
        );
    }
}
