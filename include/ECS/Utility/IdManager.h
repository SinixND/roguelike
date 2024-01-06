#ifndef IDMANAGER_H_20231210202055
#define IDMANAGER_H_20231210202055

#include "Id.h"
#include <cassert>
#include <limits>
#include <unordered_set>

namespace snd
{
    class IdManager
    {
    public:
        Id requestId()
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

            return lastId_;
        };

        void suspendId(Id id)
        {
            if (activeIds_.count(id) < 1)
            {
                return;
            };

            auto usedIdsIterator{activeIds_.find(id)};
            freeIds_.insert(*usedIdsIterator);
            activeIds_.erase(usedIdsIterator);
        };

        IdManager(Id maxId = std::numeric_limits<Id>::max())
            : maxId_(maxId){};

    private:
        const Id maxId_{};

        Id lastId_{0};

        std::unordered_set<Id> activeIds_;
        std::unordered_set<Id> freeIds_;
    };

}

#endif
