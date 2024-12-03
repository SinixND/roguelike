#ifndef IG20241028165100
#define IG20241028165100

struct AIComponent;
struct Enemies;
struct Map;
struct Vector2I;
class GameCamera;
class PositionComponent;
class TransformComponent;
class EnergyComponent;
struct DamageComponent;
class HealthComponent;

namespace AISystem
{
    //* Return if all enemies checked
    bool checkForAction(
        Enemies& enemies,
        Map const& map,
        Vector2I const& heroPosition,
        HealthComponent& heroHealth,
        GameCamera const& gameCamera);

    void chooseAction(
        AIComponent const& ai,
        PositionComponent& position,
        TransformComponent& transform,
        EnergyComponent& energy,
        DamageComponent& damage,
        Map const& map,
        Vector2I const& heroPosition,
        HealthComponent& heroHealth,
        GameCamera const& gameCamera);
}

#endif
