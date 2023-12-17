#include "EntityManager.h"
#include "IdManager.h"
#include <utility>
#include <vector>

namespace snd
{
    IdManager* idManager{IdManager::getInstance()};

    Entity EntityManager::create()
    {
        Entity entity{idManager->requestId()};
        std::vector<bool> entitySignature;
        entities_.insert(std::make_pair(entity, entitySignature));
        return entity;
    };

    void EntityManager::remove(Entity entity)
    {
        if (!entities_.count(entity))
        {
            return;
        };

        auto entitiesIterator{entities_.find(entity)};
        entities_.erase(entity);
    };
}
