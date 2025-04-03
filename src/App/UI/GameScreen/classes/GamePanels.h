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

namespace GamePanelsModule
{
    [[nodiscard]]
    GamePanels const& init( GamePanels& panels );
}

#endif
