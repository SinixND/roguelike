#ifndef IG20231210202055
#define IG20231210202055

#include <cassert>
#include <limits>
#include <unordered_set>

namespace snx
{
    class IdManager
    {
        size_t maxId_ = std::numeric_limits<size_t>::max();
        //* First requested id is initialization value + 1
        size_t lastId_{ 1 };

        std::unordered_set<size_t> activeIds_{};
        std::unordered_set<size_t> freeIds_{};

    public:
        size_t requestId()
        {
            if ( !freeIds_.empty() )
            {
                return recycledId();
            }

            return incrementedId();
        }

        void suspendId( size_t id )
        {
            if ( activeIds_.count( id ) < 1 )
            {
                return;
            }

            auto usedIdsIterator{ activeIds_.find( id ) };
            freeIds_.insert( *usedIdsIterator );
            activeIds_.erase( usedIdsIterator );
        }

    private:
        size_t recycledId()
        {
            size_t id{};
            auto freeIdsIterator{ freeIds_.begin() };
            id = *freeIdsIterator;

            activeIds_.insert( id );
            freeIds_.erase( freeIdsIterator );

            return id;
        }

        size_t incrementedId()
        {
            ++lastId_;

            assert( lastId_ < maxId_ && "ID OVERFLOWING!" );

            activeIds_.insert( lastId_ );

            return lastId_;
        }
    };
}

#endif
