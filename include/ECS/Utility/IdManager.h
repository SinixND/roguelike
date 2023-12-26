#ifndef IDMANAGER_H_20231210202055
#define IDMANAGER_H_20231210202055

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

        IdManager(Id maxId = std::numeric_limits<Id>::max());

    private:
        const Id maxId_{};

        Id lastId_{0};

        std::unordered_set<Id> activeIds_;
        std::unordered_set<Id> freeIds_;
    };

}

#endif
