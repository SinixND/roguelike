#ifndef IG20250309193224
#define IG20250309193224

#include "OverlayLevelUp.h"

struct Overlays
{
    OverlayLevelUp levelUp{};
};

namespace OverlaysModule
{
    [[nodiscard]]
    Overlays const& init( Overlays& overlays );
}

#endif
