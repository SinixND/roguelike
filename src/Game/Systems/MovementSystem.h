#ifndef IG20241027130300
#define IG20241027130300

#include "raylibEx.h"
#include <vector>

struct TransformComponent;
struct MovementComponent;
class PositionComponent;
class EnergyComponent;
struct Map;
class PositionComponent;

namespace MovementSystem
{
    //* Returns if tilePosition changed
    bool update(
        TransformComponent& transform,
        MovementComponent& movement,
        PositionComponent& position,
        EnergyComponent& energy,
        Map const& map,
        PositionComponent const& heroPosition);

    void prepareInputAgnostic(
        MovementComponent& movement,
        TransformComponent const& transform,
        PositionComponent const& position);

    void prepareByNewPath(
        MovementComponent& movement,
        PositionComponent const& position,
        std::vector<Vector2I> const& path);

    //* Triggers from and adjusts path
    void prepareFromExistingPath(
        MovementComponent& movement,
        PositionComponent const& position);

    void prepareByDirection(
        MovementComponent& movement,
        PositionComponent const& position,
        Vector2I const& direction);

    //* Sets direction, currentVelocity and isTriggered_
    void prepareByFromTo(
        MovementComponent& movement,
        PositionComponent const& position,
        Vector2I const& from,
        Vector2I const& to);

    void resetTransform(TransformComponent& transform);

    void updateCumulativeDistance(TransformComponent& transform);
    void resetCumulativeDistance(TransformComponent& transform);

    //* Calculate distance for this frame
    Vector2 frameOffset(TransformComponent const& transform);
};

#endif
