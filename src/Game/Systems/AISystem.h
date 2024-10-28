#ifndef IG20241028165100
#define IG20241028165100

class AIComponent;
struct Enemies;
struct Map;
struct Vector2I;
class GameCamera;
class PositionComponent;
class MovementComponent;
class EnergyComponent;

namespace AISystem
{
    //* Return if all enemies checked
    bool checkForAction(
        Enemies& enemies,
        Map const& map,
        Vector2I const& heroPosition,
        GameCamera const& gameCamera);

    void chooseAction(
        AIComponent const& ai,
        PositionComponent& position,
        MovementComponent& movement,
        EnergyComponent& energy,
        Map const& map,
        Vector2I const& heroPosition,
        GameCamera const& gameCamera);
}

#endif
