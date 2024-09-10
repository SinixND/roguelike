#ifndef IG20240907173344
#define IG20240907173344

#include "Enemies.h"
#include "Objects.h"
#include "Tiles.h"

class Map
{
    Tiles tiles_{};
    Objects objects_{};
    Enemies enemies_{};

public:
    Tiles& tiles() { return tiles_; };
    Objects& objects() { return objects_; };
    Enemies& enemies() { return enemies_; };
};

#endif
