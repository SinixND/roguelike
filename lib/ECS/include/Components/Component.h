#ifndef COMPONENT_H_20231219002351
#define COMPONENT_H_20231219002351

#include "Id.h"
#include "IdManager.h"

namespace snd
{
    template <typename ComponentType>
    class Component
    {
    public:
        static inline Id getId()
        {
            IdManager* idManager{IdManager::getInstance()};
            static Id id = idManager->requestId();
            return id;
        }

        Component() = default;
        virtual ~Component() = default;
        Component(const Component&) = default;
        Component& operator=(const Component&) = default;
        Component(Component&&) = default;
        Component& operator=(Component&&) = default;
    };
}

#endif
