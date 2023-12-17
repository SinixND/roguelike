#include "ECS.h"

namespace snd
{
    // Define static members
    std::unique_ptr<EntityManager> ECS::entityManager{};

    std::map<std::string, std::shared_ptr<BaseComponentManager>> ECS::componentManagers_{};
}