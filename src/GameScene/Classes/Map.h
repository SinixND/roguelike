#ifndef IG20240907173344
#define IG20240907173344

#include "Objects.h"
#include "Tiles.h"

class Map
{
    Tiles tiles_{};
    Objects objects_{};

public:
    Tiles& tiles() { return tiles_; };
    Objects& objects() { return objects_; };
};

#endif