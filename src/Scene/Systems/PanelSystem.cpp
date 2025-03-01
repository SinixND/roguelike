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
            .setInner(
                RectangleEx{
                    0,
                    0,
                    static_cast<float>( GetRenderWidth() ),
                    //* LineCount + (max(padding, border))
                    ( 1 * ( 1.5f * charSize.y ) )
                },
                0.5f * charSize.y
            );

        panels.log
            .setInner(
                RectangleEx{
                    panels.status.outer().left(),
                    panels.status.outer().bottom(),
                    static_cast<float>( GetRenderWidth() ),
                    ( 3 * ( 1.5f * charSize.y ) )
                },
                0.5f * charSize.y
            );

        panels.info
            .setInner(
                RectangleEx{
                    panels.status.outer().left(),
                    panels.status.outer().bottom(),
                    static_cast<float>( GetRenderWidth() ),
                    ( 2 * charSize.y )
                },
                0.5f * charSize.y
            );

        return panels;
    }

    void drawPanelBorders( Panels const& panels )
    {
        float borderWidth{ 1.0f };

        DrawRectangleLinesEx(
            panels.status.box().rectangle(),
            borderWidth,
            Color{ 25, 25, 25, 255 }
        );

        DrawRectangleLinesEx(
            panels.info.box().rectangle(),
            borderWidth,
            Color{ 25, 25, 25, 255 }
        );

        DrawRectangleLinesEx(
            panels.log.box().rectangle(),
            borderWidth,
            Color{ 25, 25, 25, 255 }
        );

        DrawRectangleLinesEx(
            panels.map.box().rectangle(),
            borderWidth,
            Color{ 25, 25, 25, 255 }
        );
    }
}
