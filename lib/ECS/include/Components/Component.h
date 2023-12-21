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
            static Id id{componentTypeIdManager_.requestId()}; // initialized only once because it is static
            return id;
        }

        Component() = default;
        virtual ~Component() = default;
        Component(const Component&) = default;
        Component& operator=(const Component&) = default;
        Component(Component&&) = default;
        Component& operator=(Component&&) = default;

    private:
        static inline IdManager componentTypeIdManager_;
    };
}

#endif
