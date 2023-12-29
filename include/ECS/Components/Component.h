#ifndef COMPONENT_H_20231219002351
#define COMPONENT_H_20231219002351

#include "Id.h"
#include "Signature.h"

namespace snd
{
    struct BaseComponent
    {
        static inline Id typeId{0};
        virtual ~BaseComponent() = default;
    };

    template <typename ComponentType>
    class Component : public BaseComponent
    {
    public:
        static inline Id getId()
        {
            static Id id{++typeId}; // initialized only once per templated type because it is static
            return id;
        }
    };
}

#endif
