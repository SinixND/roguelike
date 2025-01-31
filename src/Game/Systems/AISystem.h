#ifndef IG_AISystem_20250104200416
#define IG_AISystem_20250104200416

#include <cstddef>

class Enemies;
struct Hero;
struct Map;
struct GameCamera;

namespace AISystem
{
    //* Return if multi-frame action taken
    bool executeNextAction(
        size_t* activeEnemyIdIO,
        Enemies* enemiesIO,
        Hero* heroIO,
        Map const& map,
        GameCamera const& gameCamera,
        int turn
    );
}

#endif
