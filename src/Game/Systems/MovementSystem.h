#ifndef IG20241027130300
#define IG20241027130300

class EnergyComponent;
struct Map;
struct MovementComponent;
class PositionComponent;
struct TransformComponent;

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
    //* - Resets currentVelocity
    //* Returns if tilePosition changed
    bool update(
        TransformComponent& transform,
        MovementComponent& movement,
        PositionComponent& position,
        EnergyComponent& energy,
        Map const& map,
        PositionComponent const& heroPosition);
};

#endif
