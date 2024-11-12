#ifndef IG20231210202055
#define IG20231210202055

#include <cassert>
#include <cstddef>
#include <limits>
#include <unordered_set>

namespace snx
{
    class IDManager
    {
        std::unordered_set<size_t> activeIDs_{};
        std::unordered_set<size_t> freeIDs_{};

    public:
        size_t requestID()
        {
            if (!freeIDs_.empty())
            {
                return recycledID();
            }

            return incrementedID();
        }

        void suspendID(size_t id)
        {
            if (activeIDs_.count(id) < 1)
            {
                return;
            }

            auto usedIDsIterator{activeIDs_.find(id)};
            freeIDs_.insert(*usedIDsIterator);
            activeIDs_.erase(usedIDsIterator);
        }

    private:
        size_t recycledID()
        {
            size_t id{};
            auto freeIDsIterator{freeIDs_.begin()};
            id = *freeIDsIterator;

            activeIDs_.insert(id);
            freeIDs_.erase(freeIDsIterator);

            return id;
        }

        size_t incrementedID()
        {
            [[maybe_unused]] static size_t maxID = std::numeric_limits<size_t>::max();
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