#ifndef IG_AISystem_20250104200416
#define IG_AISystem_20250104200416

#include <cstddef>

struct AIComponent;
struct World;
struct Hero;
struct Map;
struct Vector2I;
struct GameCamera;
struct Vector2;
struct TransformComponent;
struct MovementComponent;
struct EnergyComponent;
struct DamageComponent;
struct HealthComponent;

namespace AISystem
{
    bool takeActions(
        World& world,
        Hero& hero,
        size_t& activeEnemyId,
        GameCamera const& gameCamera,
        int turn);

    //* Return if multi-frame action taken
    bool takeAction(
        AIComponent const& ai,
        Vector2& position,
        MovementComponent& movement,
        TransformComponent& transform,
        EnergyComponent& energy,
        DamageComponent& damage,
        Map const& map,
        Vector2I const& heroPosition,
        HealthComponent& heroHealth,
        GameCamera const& gameCamera);
}

#endif
