#ifndef IG20250323231827
#define IG20250323231827

#include <cstddef>

class Enemies;
struct Hero;
struct Map;
struct GameCamera;

namespace AISystem
{
    //* Return if multi-frame action taken
    [[nodiscard]]
    Enemies const& executeNextAction(
        Enemies& enemies,
        size_t& activeEnemyIdIO,
        Hero& heroIO,
        Map const& map,
        GameCamera const& gameCamera
    );
}

#endif
