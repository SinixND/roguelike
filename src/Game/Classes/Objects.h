#ifndef IG20240908213935
#define IG20240908213935

#include "DenseMap.h"
#include "EventId.h"
#include "PositionComponent.h"
#include "RenderId.h"
#include "raylibEx.h"
#include <string>

struct Objects
{
    snx::DenseMap<Vector2I, PositionComponent> positions{};
    snx::DenseMap<Vector2I, RenderId> renderIds{};
    snx::DenseMap<Vector2I, std::string> names{};
    snx::DenseMap<Vector2I, std::string> actions{};
    snx::DenseMap<Vector2I, EventId> events{};
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
