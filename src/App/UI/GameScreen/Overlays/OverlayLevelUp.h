#ifndef IG20250309185723
#define IG20250309185723

#include "Panel.h"

enum class InputId;
struct Game;

struct OverlayLevelUp
{
    Panel panel{};
};

namespace OverlayLevelUpModule
{
    [[nodiscard]]
    OverlayLevelUp const& init( OverlayLevelUp& overlay );

    [[nodiscard]]
    OverlayLevelUp const& update(
        OverlayLevelUp& overlay,
        Game const& game
    );
}
#endif
