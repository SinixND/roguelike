#ifndef IG20250506224430
#define IG20250506224430

struct Hero;
struct Enemies;

/// Game logic: Discrete moves (tile by tile)
namespace MovementSystem
{
    void update(
        Hero& heroIO,
        Enemies& enemiesIO
    );
}

#endif
