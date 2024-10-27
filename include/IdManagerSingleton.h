#ifndef IG20231210202056
#define IG20231210202056

#include "Id.h"
#include "Singleton.h"
#include <cassert>
#include <cstddef>
#include <limits>
#include <unordered_set>

namespace snx
{
    class IdManager : public snx::Singleton<IdManager>
    {
        static inline size_t const MAX_ID{std::numeric_limits<Id>::max()};
        static inline size_t lastId_{0};
        static inline std::unordered_set<Id> activeIds_;
        static inline std::unordered_set<Id> freeIds_;

    public:
        static Id requestId()
        {
            assert(lastId_ < MAX_ID && "ID OVERFLOWING!");

            if (!freeIds_.empty())
            {
                return recycledId();
            }

            return incrementedId();
        }

        static void suspendId(Id id)
        {
            if (activeIds_.count(id) < 1)
            {
                return;
            };

            auto usedIdsIterator{activeIds_.find(id)};
            freeIds_.insert(*usedIdsIterator);
            activeIds_.erase(usedIdsIterator);
        }

    private:
        static Id recycledId()
        {
            size_t id{};
            auto freeIdsIterator{freeIds_.begin()};
            id = *freeIdsIterator;

            activeIds_.insert(id);
            freeIds_.erase(freeIdsIterator);

            return id;
        }

        static Id incrementedId()
        {
            static size_t maxId = std::numeric_limits<size_t>::max();
            static size_t lastId{0};

            //* Start with id = 1
            ++lastId;

            assert(lastId < maxId && "ID OVERFLOWING!");

            activeIds_.insert(lastId);

            return lastId;
        }
    };
}

#endif