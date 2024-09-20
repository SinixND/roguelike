#ifndef IG20240907173344
#define IG20240907173344

#include "Enemies.h"
#include "Objects.h"
#include "Tiles.h"

struct Map
{
    Tiles tiles_{};
    Objects objects_{};
    Enemies enemies_{};
};

#endif
