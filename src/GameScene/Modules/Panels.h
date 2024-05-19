#ifndef IG20240407004107
#define IG20240407004107

#include "FontProperties.h"
#include "TileMap.h"
#include "raylibEx.h"
#include <raylib.h>

namespace Panels
{
    // Panels (sorted by descending priority)
    float constexpr PANEL_BORDER_WEIGHT{1};

    // Info (right); Number: chars per line
    float constexpr INFO_PANEL_WIDTH{(10 /*Number of chars per line*/ * FontProperties::FONT_SIZE) + FontProperties::FONT_SIZE};

    // Status (top); Number: lines to print
    float constexpr STATUS_PANEL_HEIGHT{(1 /*Number of lines to print*/ * (1.5F * FontProperties::FONT_SIZE)) + (0.5F * FontProperties::FONT_SIZE)};

    // Log (bottom);
    float constexpr LOG_PANEL_HEIGHT{(3 /*Number of lines to print*/ * (1.5F * FontProperties::FONT_SIZE)) + (0.5F * FontProperties::FONT_SIZE)};
}

namespace PanelTileInfo
{
    RectangleEx setup();

    void render(TileMap* tileMap, Vector2I cursorPosition, Font const& font);
}

namespace PanelInfo
{
    RectangleEx setup();
}

namespace PanelStatus
{
    RectangleEx setup();

    void render(int level, Font const& font);
}

namespace PanelLog
{
    RectangleEx setup();
}

namespace PanelMap
{
    RectangleEx setup();
}
#endif
