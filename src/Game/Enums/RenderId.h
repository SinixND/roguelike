#ifndef IG20240209175845
#define IG20240209175845

#include <string>
#include <unordered_map>

enum class RenderId
{
    NONE,
    //* UI
    CURSOR,
    //* Tiles
    WALL,
    FLOOR,
    //* Objects
    DESCEND,
    ASCEND,
    //* Units
    HERO,
    //* Enemies
    GOBLIN,
    //* Misc
};

inline std::unordered_map<RenderId, std::string> const enemyNames{
    { RenderId::GOBLIN, "Goblin" },
};

#endif
