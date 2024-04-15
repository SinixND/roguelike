#ifndef IG20240308011247
#define IG20240308011247

#include "GameObject.h"
#include "Pathfinder.h"
#include "Unit.h"
#include "World.h"

namespace MapOverlay
{
    void update(Unit& hero, World& gameWorld, GameObject& cursor);
    Path& path();
}

#endif