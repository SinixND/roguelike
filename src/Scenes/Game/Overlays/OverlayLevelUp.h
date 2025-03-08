#ifndef IG20250309185723
#define IG20250309185723

#include "Panel.h"

struct OverlayLevelUp
{
    Panel panel{};
};

namespace OverlayLevelUpModule
{
    [[nodiscard]]
    OverlayLevelUp const& init( OverlayLevelUp& overlay );

    void update( OverlayLevelUp const& overlay );
}
#endif
