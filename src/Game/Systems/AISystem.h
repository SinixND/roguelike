#ifndef IG20241028165100
#define IG20241028165100

#include <cstddef>

struct AIComponent;
class World;
struct Hero;
struct Map;
struct Vector2I;
class GameCamera;
class PositionComponent;
struct TransformComponent;
struct MovementComponent;
class EnergyComponent;
struct DamageComponent;
class HealthComponent;

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
        PositionComponent& position,
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
