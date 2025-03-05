#ifndef IG20241027130300
#define IG20241027130300

#include "MovementComponent.h"
#include "raylibEx.h"
#include <vector>

struct TransformComponent;
struct Vector2;
struct EnergyComponent;

namespace MovementSystem
{
    [[nodiscard]]
    Vector2 const& updateSingle(
        Vector2& positionIO,
        TransformComponent& transformIO,
        // MovementComponent& movementIO,
        float dt
    );

    [[nodiscard]]
    TransformComponent const& prepareByDirection(
        TransformComponent& transform,
        MovementComponent const& movement,
        Vector2I const& direction
    );

    [[nodiscard]]
    MovementComponent const& prepareByNewPath(
        MovementComponent& movement,
        std::vector<Vector2I> const& path
    );

    //* Triggers from and adjusts path
    [[nodiscard]]
    TransformComponent const& prepareFromExistingPath(
        TransformComponent& transform,
        MovementComponent& movementIO
    );

    //* Sets direction, currentVelocity and isTriggered_
    [[nodiscard]]
    TransformComponent const& prepareByFromTo(
        TransformComponent& transform,
        MovementComponent const& movement,
        Vector2I const& from,
        Vector2I const& to
    );

    [[nodiscard]]
    TransformComponent const& resetTransform( TransformComponent& transform );
};

#endif
