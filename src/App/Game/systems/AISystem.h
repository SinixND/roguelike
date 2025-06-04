#ifndef IG20250323231827
#define IG20250323231827

#include <cstddef>

struct Enemies;
struct Hero;
struct Map;
struct GameCamera;

namespace AISystem
{
    void update(
        Enemies& enemiesIO,
        Map const& map,
        Hero const& hero,
        GameCamera const& gameCamera
    );
}

#endif
