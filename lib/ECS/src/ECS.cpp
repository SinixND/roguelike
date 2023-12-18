#include "ECS.h"

namespace snd
{
    // Define static members
    ContiguousContainer<EntityId, std::set<Id>> ECS::entityMaskComponents_{};

    IdManager* ECS::entityManager{};

    std::unordered_map<std::string, std::shared_ptr<BaseComponentManager>> ECS::componentManagers_{};
}