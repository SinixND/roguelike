#ifndef SYSTEMMANAGER_H_20240103210800
#define SYSTEMMANAGER_H_20240103210800

#include "System.h"
#include "SystemId.h"
#include <memory>
#include <unordered_set>
#include <vector>

namespace snd
{
    class SystemManager
    {
    public:
        template <typename SystemType>
        SystemType* registerSystem(ComponentManager& componentManager)
        {
            auto system{std::make_unique<SystemType>(componentManager)};

            systems_.push_back(system);
            return &system;
        }

        std::vector<std::shared_ptr<BaseSystem>>* retrieveSystems()
        {
            return &systems_;
        }

    private:
        std::vector<std::shared_ptr<BaseSystem>> systems_;
    };
}

#endif