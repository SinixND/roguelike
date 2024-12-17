#ifndef IG20240914212555
#define IG20240914212555

class Tiles;
struct Enemies;
struct Objects;
struct Vector2I;

namespace CollisionSystem
{
    //* Checks for
    //* - solid tiles
    //* - enemies
    //* Returns if collision occurs
    bool checkCollision(
        Tiles const& tiles,
        Enemies const& enemies,
        // Objects const& objects,
        Vector2I const& tilePositionToCheck,
        Vector2I const& heroPosition);
}

#endif
