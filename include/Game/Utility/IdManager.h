#ifndef _20231210202055
#define _20231210202055

#include "Id.h"
#include <limits>
#include <unordered_set>

namespace snd
{
    class IdManager
    {
    public:
        Id requestId();

        void suspendId(Id id);

        IdManager(Id maxId = std::numeric_limits<Id>::max())
            : maxId_(maxId){};

    private:
        const Id maxId_{std::numeric_limits<Id>::max()};

        Id lastId_{0};

        std::unordered_set<Id> activeIds_;
        std::unordered_set<Id> freeIds_;
    };
}

#endif
