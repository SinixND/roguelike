#ifndef IG20231210202056
#define IG20231210202056

#include "ID.h"
#include "Singleton.h"
#include <cassert>
#include <cstddef>
#include <limits>
#include <unordered_set>

namespace snx
{
    class IDManager : public snx::Singleton<IDManager>
    {
        static inline size_t const maxID{std::numeric_limits<ID>::max()};
        static inline size_t lastID_{0};
        static inline std::unordered_set<ID> activeIDs_;
        static inline std::unordered_set<ID> freeIDs_;

    public:
        static ID requestID()
        {
            assert(lastID_ < maxID && "ID OVERFLOWING!");

            if (!freeIDs_.empty())
            {
                return recycledID();
            }

            return incrementedID();
        }

        static void suspendID(ID id)
        {
            if (activeIDs_.count(id) < 1)
            {
                return;
            };

            auto usedIDsIterator{activeIDs_.find(id)};
            freeIDs_.insert(*usedIDsIterator);
            activeIDs_.erase(usedIDsIterator);
        }

    private:
        static ID recycledID()
        {
            size_t id{};
            auto freeIDsIterator{freeIDs_.begin()};
            id = *freeIDsIterator;

            activeIDs_.insert(id);
            freeIDs_.erase(freeIDsIterator);

            return id;
        }

        static ID incrementedID()
        {
            static size_t maxID = std::numeric_limits<size_t>::max();
            static size_t lastID{0};

            //* Start with id = 1
            ++lastID;

            assert(lastID < maxID && "ID OVERFLOWING!");

            activeIDs_.insert(lastID);

            return lastID;
        }
    };
}

#endif