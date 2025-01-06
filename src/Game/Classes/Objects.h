#ifndef IG20240908213935
#define IG20240908213935

#include "DenseMap.h"
#include "EventId.h"
#include "IdManager.h"
#include "RenderId.h"
#include "SparseSet.h"
#include "raylibEx.h"
#include <string>

struct Objects
{
    snx::IdManager idManager{};
    snx::DenseMap<Vector2I, size_t> ids{};

    snx::SparseSet<Vector2> positions{};
    snx::SparseSet<RenderId> renderIds{};
    snx::SparseSet<std::string> names{};
    snx::SparseSet<std::string> actions{};
    snx::SparseSet<EventId> events{};
};

namespace ObjectsModule
{
    void createSingle(
        Objects& objects,
        Vector2I const& tilePosition,
        RenderId renderId,
        std::string const& name,
        std::string const& action,
        EventId event);
}

#endif
