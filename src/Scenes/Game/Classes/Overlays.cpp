#include "Overlays.h"

namespace OverlaysModule
{
    [[nodiscard]]
    Overlays const& init( Overlays& overlays )
    {
        overlays.levelUp = OverlayLevelUpModule::init( overlays.levelUp );

        return overlays;
    }
}

