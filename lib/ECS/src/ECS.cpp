#include "ECS.h"

namespace snd
{
    // Define static members
    ContiguousContainer<Entity, std::set<Id>> ECS::entityMaskComponents_{};

    std::unique_ptr<EntityManager> ECS::entityManager{};

    std::unordered_map<std::string, std::shared_ptr<BaseComponentManager>> ECS::componentManagers_{};
}