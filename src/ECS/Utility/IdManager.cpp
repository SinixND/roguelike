#include "IdManager.h"

#include "Id.h"
#include <assert.h>
#include <limits> // max value of type

namespace snd
{
    IdManager::IdManager(Id maxId)
        : maxId_{maxId} {};

    Id IdManager::requestId()
    {
        assert(lastId_ < maxId_ && "ID OVERFLOWING!");

        Id id;

        if (!freeIds_.empty())
        {
            auto freeIdsIterator{freeIds_.begin()};
            id = *freeIdsIterator;
            activeIds_.insert(id);
            freeIds_.erase(freeIdsIterator);

            return id;
        }

        // start with id = 1
        ++lastId_;

        activeIds_.insert(lastId_);

        return lastId_;
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
}
