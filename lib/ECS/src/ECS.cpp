#include "ECS.h"
#include "Id.h"

namespace snd
{
    // Define static members
    ContiguousContainer<EntityId, std::set<Id>> ECS::entityMaskComponents_{};

    IdManager* ECS::entityManager{};

    std::unordered_map<Id, std::shared_ptr<BaseComponentManager>> ECS::componentManagers_{};
}