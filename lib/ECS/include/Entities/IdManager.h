#ifndef ENTITYMANAGER_H_20231210202055
#define ENTITYMANAGER_H_20231210202055

#include "Id.h"
#include <cstdint> // uint32_t
#include <mutex>   // thread locks
#include <set>     // std::set

#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete;           \
    T& operator=(const T&) = delete;

namespace snd
{
    class IdManager
    {
    public:
        static Id requestId();
        static std::set<Id>& retrieveActiveIds();
        static void suspendId(Id id);

        // Singleton attributes
        //---------------------------------
        static IdManager* getInstance();
        //---------------------------------

    private:
        static std::set<Id> activeIds_;
        static std::set<Id> freeIds_;

        static Id lastId_;

        // Singleton attributes
        //---------------------------------
        static IdManager* singleton_;
        static std::mutex mutex_;
        IdManager(){};
        ~IdManager(){};
        DISALLOW_COPY_AND_ASSIGN(IdManager)
        //---------------------------------
    };

}

#endif
