#ifndef IG20250309185723
#define IG20250309185723

#include "Panel.h"

enum class InputId;
struct Hero;

struct OverlayLevelUp
{
    Panel panel{};
    int selectedAttribute{ 0 };
};

namespace OverlayLevelUpModule
{
    [[nodiscard]]
    OverlayLevelUp const& init( OverlayLevelUp& overlay );

    [[nodiscard]]
    OverlayLevelUp const& update(
        OverlayLevelUp& overlay,
        Hero& heroIO,
        InputId currentInputId
    );
}
#endif
