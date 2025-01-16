#ifndef IG20241027130300
#define IG20241027130300

#include "raylibEx.h"
#include <vector>

struct TransformComponent;
struct MovementComponent;
struct Vector2;
struct EnergyComponent;
struct Map;
struct Enemies;

namespace MovementSystem
{
    void updateHero(
        TransformComponent& transform,
        // MovementComponent& movement,
        Vector2& position,
        EnergyComponent& energy,
        Vector2 const& heroPosition
    );

    void updateEnemies(
        Enemies& enemies,
        Vector2 const& heroPosition
    );

    void prepareByDirection(
        MovementComponent const& movement,
        Vector2I const& direction,
        TransformComponent& transform
    );

    void prepareByNewPath(
        MovementComponent& movement,
        std::vector<Vector2I> const& path
    );

    //* Triggers from and adjusts path
    void prepareFromExistingPath(
        MovementComponent& movement,
        // Vector2 const& position);
        TransformComponent& transform
    );

    //* Sets direction, currentVelocity and isTriggered_
    void prepareByFromTo(
        MovementComponent& movement,
        TransformComponent& transform,
        Vector2I const& from,
        Vector2I const& to
    );

    void resetTransform( TransformComponent& transform );

    void resetCumulativeDistance( TransformComponent& transform );

    //* Calculate distance for this frame
    Vector2 frameOffset( TransformComponent const& transform );
};

#endif
