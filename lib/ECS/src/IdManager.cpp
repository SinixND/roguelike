#include "IdManager.h"
#include "Id.h"
#include <assert.h>
#include <iostream>
#include <limits> // max value of type
#include <set>

namespace snd
{
    std::set<Id> IdManager::usedIds_{};
    std::set<Id> IdManager::freeIds_{};
    Id IdManager::lastId_{1};

    Id IdManager::requestId()
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
        assert(lastId_ < std::numeric_limits<Id>::max() && "ID OVERFLOWING!");
        ++lastId_;
        usedIds_.insert(lastId_);

        return lastId_;
    };

    void IdManager::suspendId(Id id)
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
