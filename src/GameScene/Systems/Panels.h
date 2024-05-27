#ifndef IG20240407004107
#define IG20240407004107

#include "Event.h"
#include "FontProperties.h"
#include "ISubscriber.h"
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
    void update(RectangleEx* panelTileInfo);

    void render(RectangleEx const& panelTileInfo, TileMap& tileMap, Vector2I cursorPosition, Font const& font);
}

namespace PanelInfo
{
    void update(RectangleEx* panelInfo, RectangleEx const& panelTileInfo);
}

namespace PanelStatus
{
    void update(RectangleEx* panelStatus, RectangleEx const& panelTileInfo);

    void render(RectangleEx const& panelMap, int level, Font const& font);
}

namespace PanelLog
{
    void update(RectangleEx* panelLog, RectangleEx const& panelTileInfo);
}

namespace PanelMap
{
    void update(
        RectangleEx* panelMap,
        RectangleEx const& panelTileInfo,
        RectangleEx const& panelLog,
        RectangleEx const& panelStatus);
}

namespace PanelMapExtended
{
    void update(RectangleEx* panelMapExtended, RectangleEx const& panelMap);
}

namespace Panels
{
    // Functor
    class SubUpdatePanels : public ISubscriber
    {
    public:
        void onNotify() override;

        SubUpdatePanels(
            Event event,
            RectangleEx& panelTileInfo,
            RectangleEx& panelInfo,
            RectangleEx& panelStatus,
            RectangleEx& panelLog,
            RectangleEx& panelMap,
            RectangleEx& panelMapExtended)
            : ISubscriber(event)
            , panelTileInfo_(panelTileInfo)
            , panelInfo_(panelInfo)
            , panelStatus_(panelStatus)
            , panelLog_(panelLog)
            , panelMap_(panelMap)
            , panelMapExtended_(panelMapExtended)
        {
        }

    private:
        RectangleEx& panelTileInfo_;
        RectangleEx& panelInfo_;
        RectangleEx& panelStatus_;
        RectangleEx& panelLog_;
        RectangleEx& panelMap_;
        RectangleEx& panelMapExtended_;
    };
}
#endif
