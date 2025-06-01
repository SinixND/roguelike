#ifndef IG20250506224430
#define IG20250506224430

struct Hero;
struct Enemies;

namespace MoveSystem
{
    void update(
        Hero& heroIO,
        Enemies& enemiesIO,
        float dt
    );
}

#endif
