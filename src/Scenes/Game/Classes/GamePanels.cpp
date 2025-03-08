#include "GamePanels.h"

#include "GameFont.h"
#include "GamePanels.h"
#include "raylibEx.h"
#include <raygui.h>
#include <raylib.h>
#include <raymath.h>

namespace GamePanelsModule
{
    GamePanels const& init( GamePanels& panels )
    {
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
                    ( 1 * GameFont::fontSize ) + GameFont::fontSize
                },
                0,
                0.5f * GameFont::fontSize
            );

        panels.info
            .setOuter(
                RectangleEx{
                    panels.status.outer().left(),
                    panels.status.outer().bottom(),
                    static_cast<float>( GetRenderWidth() ),
                    ( 1 * GameFont::fontSize ) + GameFont::fontSize
                },
                0,
                0.5f * GameFont::fontSize
            );

        panels.log
            .setOuter(
                RectangleEx{
                    panels.info.outer().left(),
                    panels.info.outer().bottom(),
                    static_cast<float>( GetRenderWidth() ),
                    ( 3 * GameFont::fontSize ) + GameFont::fontSize
                },
                0,
                0.5f * GameFont::fontSize
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
