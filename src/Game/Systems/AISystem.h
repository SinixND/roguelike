#ifndef IG20241028165100
#define IG20241028165100

struct AIComponent;
struct Enemies;
struct Map;
struct Vector2I;
class GameCamera;
struct Vector2;
struct TransformComponent;
struct EnergyComponent;
struct DamageComponent;
struct HealthComponent;

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
        Vector2& position,
        TransformComponent& movement,
        EnergyComponent& energy,
        DamageComponent& damage,
        Map const& map,
        Vector2I const& heroPosition,
        HealthComponent& heroHealth,
        GameCamera const& gameCamera);
}

#endif
