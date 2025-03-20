#ifndef IG20250320163415
#define IG20250320163415

struct Hero;
struct Map;
struct Vector2I;

namespace CombatSystem
{
    void performAttack(
        Hero& heroIO,
        Map& mapIO,
        Vector2I const& target
    );
}

#endif
