#ifndef IG20240308011247
#define IG20240308011247

#include "GameObject.h"
#include "Pathfinder.h"
#include "Unit.h"
#include "World.h"

namespace MapOverlay
{
    void update(Unit const& hero, World* gameWorld, GameObject const& cursor);
    Path const& path();
}

#endif