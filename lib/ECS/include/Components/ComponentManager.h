#ifndef COMPONENTMANAGER_H_20231212234818
#define COMPONENTMANAGER_H_20231212234818

#include "Container.h"
#include "Entity.h"
#include "Id.h"
#include "IdManager.h"
#include <cassert>
#include <functional>
#include <map>
#include <vector>

typedef size_t Index;

namespace snd
{
    class BaseComponentManager
    {
    public:
        BaseComponentManager() = default;
        virtual ~BaseComponentManager() = default;
        BaseComponentManager(const BaseComponentManager&) = default;
        BaseComponentManager& operator=(const BaseComponentManager&) = default;
        BaseComponentManager(BaseComponentManager&&) = default;
        BaseComponentManager& operator=(BaseComponentManager&&) = default;
    };

    template <typename ComponentType>
    class ComponentManager : public BaseComponentManager
    {
    public:
        Id getComponentTypeId() { return componentTypeId_; };

        // Add a component to an entity
        void assignTo(Entity entity, ComponentType component)
        {
            assignedComponents_.addElement(entity, component);
        };

        // Access a component from a specific entity
        ComponentType& retrieveFrom(Entity entity)
        {
            return assignedComponents_.retrieveElement(entity);
        };

        // Remove a component from an entity
        void removeFrom(Entity entity)
        {
            assignedComponents_.removeElement(entity);
        };

        // 4. Iterate over all items
        // void iterateAll(std::function<void(ComponentType component)> lambda)
        //{
        // assignedComponents_.iterateAllElements(lambda);
        //};

        ComponentManager()
        {
            IdManager* idManager{IdManager::getInstance()};

            componentTypeId_ = idManager->requestId();
        };

    private:
        Id componentTypeId_;

        Container<Id, ComponentType> assignedComponents_;
    };

}

#endif
