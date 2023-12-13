#include "EntityManager.h"
#include "IdManager.h"

namespace snd
{
    IdManager* idManager{IdManager::getInstance()};

    Entity EntityManager::createEntity()
    {
        Entity entity{idManager->createId()};
        entities_.insert(entity);
        return entity;
    };

    void EntityManager::removeEntity(Entity entity)
    {
        if (entities_.count(entity) < 1)
        {
            return;
        };

        auto entitiesIterator{entities_.find(entity)};
        entities_.erase(entity);
    };

}