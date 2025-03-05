#ifndef IG20250305221232
#define IG20250305221232

#include "Panel.h"

struct SceneGameOver
{
    Panel panel{};
};

namespace SceneGameOverModule
{
    [[nodiscard]]
    SceneGameOver const& init( SceneGameOver& scene );

    [[nodiscard]]
    SceneGameOver const& update( SceneGameOver& scene );
}
#endif
