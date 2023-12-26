#ifndef SYSTEM_H_20231226184152
#define SYSTEM_H_20231226184152

#include "ComponentManager.h"
#include "Id.h"
#include "Signature.h"
#include <iostream>
#include <memory>
#include <unordered_map>

namespace snd
{
    struct BaseSystem
    {
        static inline Id typeId{0};
        virtual void execute() = 0;
    };

    template <typename SystemType>
    class System : public BaseSystem
    {
    public:
        Signature signature_{0};

        void execute()
        {
            std::cout << "DUMMY: Execute systems action...\n";
            action();
        }

        virtual void action(){};

        static inline Id getId()
        {
            static Id id{++typeId}; // initialized only once per templated type because it is static
            return id;
        }

        System() = default;
        virtual ~System() = default;
        System(const System&) = default;
        System& operator=(const System&) = default;
        System(System&&) = default;
        System& operator=(System&&) = default;
    };
}

#endif
