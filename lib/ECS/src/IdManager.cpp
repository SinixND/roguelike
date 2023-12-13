#include "IdManager.h"
#include "Id.h"
#include <iostream>
#include <set>

namespace snd
{
    std::set<Id> IdManager::usedIds_{};
    std::set<Id> IdManager::freeIds_{};
    Id IdManager::lastId_{0};

    Id IdManager::createId()
    {
        Id id;

        if (!freeIds_.empty())
        {
            auto freeIdsIterator{freeIds_.begin()};
            id = *freeIdsIterator;
            usedIds_.insert(id);
            freeIds_.erase(freeIdsIterator);

            return id;
        }

        ++lastId_;
        usedIds_.insert(lastId_);

        return lastId_;
    };

    void IdManager::removeId(Id id)
    {
        if (usedIds_.count(id) < 1)
        {
            return;
        };

        auto usedIdsIterator{usedIds_.find(id)};
        freeIds_.insert(*usedIdsIterator);
        usedIds_.erase(usedIdsIterator);
    };

    // Singleton
    //-------------------------------------
    IdManager* IdManager::singleton_{nullptr};
    std::mutex IdManager::mutex_;

    IdManager* IdManager::getInstance()
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (singleton_ == nullptr)
        {
            singleton_ = new IdManager();
        }

        return singleton_;
    }
    //-------------------------------------
}