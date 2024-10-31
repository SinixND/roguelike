#ifndef IG20241028165100
#define IG20241028165100

class AIComponent;
struct EnemySoA;
struct Map;
struct Vector2I;
class GameCamera;
class PositionComponent;
class MovementComponent;
class EnergyComponent;
class DamageComponent;
class HealthComponent;

namespace AISystem
{
    //* Return if all enemies checked
    bool checkForAction(
        EnemySoA& enemies,
        Map const& map,
        Vector2I const& heroPosition,
        HealthComponent& heroHealth,
        GameCamera const& gameCamera);

    void chooseAction(
        AIComponent const& ai,
        PositionComponent& position,
        MovementComponent& movement,
        EnergyComponent& energy,
        DamageComponent& damage,
        Map const& map,
        Vector2I const& heroPosition,
        HealthComponent& heroHealth,
        GameCamera const& gameCamera);
}

#endif
