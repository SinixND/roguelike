#ifndef IG20240531191635
#define IG20240531191635

#include "Map.h"
#include "MapGeneratorSystem.h"
#include <vector>

/// World holds maps which are identified by a mapLevel (int)
struct World
{
#if defined( DEBUG )
    std::vector<Map> maps{
        MapGeneratorSystem::createTestRoom(),
        MapGeneratorSystem::createRandomMapType( 1 )
    };
#else
    std::vector<Map> maps{
        MapGeneratorSystem::createStartRoom(),
        MapGeneratorSystem::createRandomMapType( 1 )
    };
#endif

    int maxMapLevel{};

    int currentMapLevel{};
    Map* currentMap{ &maps.front() };
};

namespace WorldModule
{
    [[nodiscard]]
    World const& increaseMapLevel( World& world );

    [[nodiscard]]
    World const& decreaseMapLevel( World& world );

    [[nodiscard]]
    World const& addNewMap( World& world );

    [[nodiscard]]
    World const& activateCurrentMap(
        World& world,
        int mapLevel
    );
}
#endif
