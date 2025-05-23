#ifndef IG20240914212555
#define IG20240914212555

struct Tiles;
struct Enemies;
struct Objects;
struct Vector2I;

namespace CollisionSystem
{
    //* Returns if collision occurs
    bool checkCollisionForHero(
        Tiles const& tiles,
        Enemies const& enemies,
        // Objects const& objects,
        Vector2I const& tilePositionToCheck
    );

    //* Returns if collision occurs
    bool checkCollisionForEnemy(
        Tiles const& tiles,
        Enemies const& enemies,
        // Objects const& objects,
        Vector2I const& tilePositionToCheck,
        Vector2I const& heroPosition
    );
}

#endif
