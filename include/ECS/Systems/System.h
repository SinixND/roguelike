#ifndef SYSTEM_H_20231226184152
#define SYSTEM_H_20231226184152

#include "Component.h"
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
        Signature& getSignature()
        {
            return signature_;
        };

        BaseSystem() = default;
        virtual ~BaseSystem() = default;
        BaseSystem(const BaseSystem&) = default;
        BaseSystem& operator=(const BaseSystem&) = default;
        BaseSystem(BaseSystem&&) = default;
        BaseSystem& operator=(BaseSystem&&) = default;

    protected:
        Signature signature_{0};
    };

    template <typename SystemType>
    class System : public BaseSystem
    {
    public:
        template <typename Type, typename... ArgTypes>
        void action(Type, ArgTypes...) {}

        void execute()
        {
            std::cout << "DUMMY: Execute systems action...\n";
            void action();
        }

        System() = default;
        virtual ~System() = default;
        System(const System&) = default;
        System& operator=(const System&) = default;
        System(System&&) = default;
        System& operator=(System&&) = default;

    protected:
        // vector of pointers to component vectors for matching signature
        // std::vector<std::vector<ComponentType>*> vector;
    };
}

#endif
