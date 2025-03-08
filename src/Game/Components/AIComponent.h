#ifndef IG20240909151555
#define IG20240909151555

#include <cstddef>

struct AIComponent
{
    int scanRange{ 0 };
};

class Enemies;
struct Hero;
struct Map;
struct GameCamera;

namespace AIModule
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
