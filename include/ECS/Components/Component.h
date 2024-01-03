#ifndef COMPONENT_H_20231219002351
#define COMPONENT_H_20231219002351

#include "ComponentTypeId.h"

namespace snd
{
    struct BaseComponent
    {
        virtual ~BaseComponent() = default;

    protected:
        static inline ComponentTypeId componentTypeId_{0};
    };

    template <typename ComponentType>
    class Component : public BaseComponent
    {
    public:
        static inline ComponentTypeId getId()
        {
            static ComponentTypeId id{++componentTypeId_}; // Initialized only once per templated type because it is static
            return id;
        }
    };
}

#endif
