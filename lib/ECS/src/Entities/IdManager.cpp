#include "IdManager.h"
#include "Id.h"
#include <assert.h>
#include <limits> // max value of type
#include <set>

namespace snd
{
    std::set<Id> IdManager::activeIds_{};
    std::set<Id> IdManager::freeIds_{};
    Id IdManager::lastId_{0};

    Id IdManager::requestId()
    {
        Id id;

        if (!freeIds_.empty())
        {
            auto freeIdsIterator{freeIds_.begin()};
            id = *freeIdsIterator;
            activeIds_.insert(id);
            freeIds_.erase(freeIdsIterator);

            return id;
        }
        assert(lastId_ < std::numeric_limits<Id>::max() && "ID OVERFLOWING!");
        ++lastId_;
        activeIds_.insert(lastId_);

        return lastId_;
    };

    std::set<Id>& IdManager::retrieveActiveIds()
    {
        return activeIds_;
    };

    void IdManager::suspendId(Id id)
    {
        if (activeIds_.count(id) < 1)
        {
            return;
        };

        auto usedIdsIterator{activeIds_.find(id)};
        freeIds_.insert(*usedIdsIterator);
        activeIds_.erase(usedIdsIterator);
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
