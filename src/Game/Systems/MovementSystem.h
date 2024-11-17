#ifndef IG20241027130300
#define IG20241027130300

struct TransformComponent;
struct Vector2;
struct EnergyComponent;
struct Map;
struct Vector2;

namespace MovementSystem
{
    //* Moves with following steps
    //* - Check for collision
    //* - Starts movement if trigger set OR trigger not set but path available
    //* - Consumes energy,
    //* - Sets inProgress state,
    //* - Skips if action is in progress,
    //* - Moves for one tile max
    //* - Resets inProgress state if moved for one tile
    //* - Resets velocity
    //* Returns if tilePosition changed
    bool update(
        TransformComponent& movement,
        Vector2& position,
        EnergyComponent& energy,
        Map const& map,
        Vector2 const& heroPosition);
};

#endif
