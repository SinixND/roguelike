#ifndef IG20240914212555
#define IG20240914212555

class Map;
struct Vector2I;

namespace CollisionSystem
{
    //* Checks for
    //* - solid tiles
    //* - enemies
    //* Returns if collision occurs
    bool checkCollision(
        Map const& map,
        Vector2I const& tilePositionToCheck,
        Vector2I const& heroPosition);
}

#endif