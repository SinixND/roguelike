#include "EntityManager.h"
#include "IdManager.h"

namespace snd
{
    IdManager* idManager{IdManager::getInstance()};

    Entity EntityManager::create()
    {
        Entity entity{idManager->requestId()};
        entities_.insert(entity);
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
