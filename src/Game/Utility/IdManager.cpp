#include "IdManager.h"

#include "Id.h"
#include <cassert>
#include <iostream>
#include <limits>

namespace snd
{
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

        // Start with id = 1
        ++lastId_;

        activeIds_.insert(lastId_);

        std::cout << "Provide ID: " << lastId_ << "\n";
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
