#ifndef IG20250305221232
#define IG20250305221232

#include "Panel.h"

struct ScreenGameOver
{
    Panel panelComponent{};
};

namespace ScreenGameOverModule
{
    [[nodiscard]]
    ScreenGameOver const& init( ScreenGameOver& screen );

    void update( ScreenGameOver& screen );
}
#endif
