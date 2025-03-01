#include "PanelSystem.h"

#include "GameFont.h"
#include "Panels.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

namespace PanelSystem
{
    [[nodiscard]]
    Panels const& init( Panels& panels )
    {
        Vector2 charSize{ MeasureTextEx(
            GameFont::font(),
            "I",
            GameFont::fontSize,
            0
        ) };

        panels.map.setRectangle(
            RectangleEx{
                0,
                0,
                static_cast<float>( GetRenderWidth() ),
                static_cast<float>( GetRenderHeight() )
            }
        );

        panels.status
            .setOuter(
                RectangleEx{
                    0,
                    0,
                    static_cast<float>( GetRenderWidth() ),
                    ( 1 * charSize.y ) + charSize.y
                },
                0,
                0.5f * charSize.y
            );

        panels.info
            .setOuter(
                RectangleEx{
                    panels.status.outer().left(),
                    panels.status.outer().bottom(),
                    static_cast<float>( GetRenderWidth() ),
                    ( 1 * charSize.y ) + charSize.y
                },
                0,
                0.5f * charSize.y
            );

        panels.log
            .setOuter(
                RectangleEx{
                    panels.info.outer().left(),
                    panels.info.outer().bottom(),
                    static_cast<float>( GetRenderWidth() ),
                    ( 3 * charSize.y ) + charSize.y
                },
                0,
                0.5f * charSize.y
            );

        return panels;
    }

    void drawPanelBackground( Panel const& panel, Color const& color )
    {
        DrawRectangleRec(
            panel.box().rectangle(),
            color
        );
    }

    void drawPanelBorder( Panel const& panel, float borderWidth )
    {
        DrawRectangleLinesEx(
            panel.box().rectangle(),
            borderWidth,
            Color{ 25, 25, 25, 255 }
        );
    }
}
