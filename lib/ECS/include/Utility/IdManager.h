#ifndef IDMANAGER_H_20231210202055
#define IDMANAGER_H_20231210202055

#include "Id.h"
#include <limits>
#include <set> // std::set

namespace snd
{
    class IdManager
    {
    public:
        Id requestId();
        void suspendId(Id id);

        IdManager(Id maxId = std::numeric_limits<Id>::max());

    private:
        Id maxId_{};

        Id lastId_;

        std::set<Id> activeIds_;
        std::set<Id> freeIds_;
    };

}

#endif
