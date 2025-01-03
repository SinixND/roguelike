#ifndef IG20240908213935
#define IG20240908213935

#include "EventId.h"
#include "IdManager.h"
#include "PositionComponent.h"
#include "RenderId.h"
#include "SparseSet.h"
#include "raylibEx.h"
#include <string>

struct Objects
{
    snx::SparseSet<PositionComponent> positions{};
    snx::SparseSet<RenderId> renderIds{};
    snx::SparseSet<std::string> names{};
    snx::SparseSet<std::string> actions{};
    snx::SparseSet<EventId> events{};

    snx::IdManager idManager{};
};

//* Access or create
void createObject(
    Objects& objects,
    Vector2I const& tilePosition,
    RenderId renderId,
    std::string const& name,
    std::string const& action,
    EventId event);

#endif
