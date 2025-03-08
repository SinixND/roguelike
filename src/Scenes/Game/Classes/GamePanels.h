#ifndef IG20240531174000
#define IG20240531174000

#include "Panel.h"

struct GamePanels
{
    Panel map;
    Panel status;
    Panel log;
    Panel info;
};

struct Color;
class Panel;

namespace GamePanelsModule
{
    [[nodiscard]]
    GamePanels const& init( GamePanels& panels );

    void drawPanelBackground( Panel const& panel, Color const& color );

    void drawPanelBorder( Panel const& panel, float borderWidth );
}

#endif
