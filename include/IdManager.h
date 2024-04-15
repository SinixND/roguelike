#ifndef IDMANAGER_H_20231210202055
#define IDMANAGER_H_20231210202055

#include "Id.h"
#include "Singleton.h"
#include <cassert>
#include <iostream>
#include <limits>
#include <unordered_set>

namespace snx
{
    class IdManager : public Singleton<IdManager>
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
                // std::cout << "Reuse Id:" << id << ", Active: " << activeIds_.size() << ", Free: " << freeIds_.size() << "\n";

                return id;
            }

            // Start with id = 1
            ++lastId_;

            activeIds_.insert(lastId_);

            // std::cout << "New Id:" << lastId_ << ", Active: " << activeIds_.size() << ", Free: " << freeIds_.size() << "\n";
            return lastId_;
        };

        void suspendId(Id id)
        {
            // std::cout << "Suspend " << id << "\n";
            if (activeIds_.count(id) < 1)
            {
                return;
            };

            auto usedIdsIterator{activeIds_.find(id)};
            freeIds_.insert(*usedIdsIterator);
            activeIds_.erase(usedIdsIterator);
            // std::cout << "Suspend Id:" << id << ", Active: " << activeIds_.size() << ", Free: " << freeIds_.size() << "\n";
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
