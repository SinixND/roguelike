#include "Entity.h"
#include "EntityManager.h"
#include <map>

namespace snd
{
    int EntityManager::lastEntity_{0};

    Entity EntityManager::createEntity()
    {
        ++lastEntity_;
        return Entity{lastEntity_};
    };

    // Singleton
    //-------------------------------------
    EntityManager* EntityManager::singleton_{nullptr};
    std::mutex EntityManager::mutex_;

    EntityManager* EntityManager::getInstance()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (singleton_ == nullptr)
        {
            singleton_ = new EntityManager();
        }

        return singleton_;
    }
    //-------------------------------------
}