#ifndef IG20240907173344
#define IG20240907173344

#include "Enemies.h"
#include "Objects.h"
#include "Tiles.h"

class Map
{
public:
    Tiles tiles{};
    Objects objects{};
    Enemies enemies{};
};

#endif
