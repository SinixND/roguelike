#ifndef COMPONENTMANAGER_H_20231212234818
#define COMPONENTMANAGER_H_20231212234818

#include "Component.h"
#include "ContiguousContainer.h"
#include "EntityId.h"
#include "Signature.h"
#include <cassert>
#include <functional>
#include <set>
#include <unordered_map>
#include <vector>

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
        // Add component to entiy
        void assignTo(Signature& signature, EntityId& entity, ComponentType component)
        {
            // add to signature
            components_.insert(signature).addElement(entity, component);
        };

        // Remove a component from an entity
        void removeFrom(Signature& signature, EntityId entity)
        {
            components_.at(signature).removeElement(entity);
            // delete empty signature
            if (!component_.at(signature).retrieveAllElements().size())
            {
                component_.erease(signature);
            }
        };

        // update signature associated container on entity signature change
        void updateSignature(Signature oldSignature, Signature& newSignature, Entity& entiy)
        {
            // move from oldSignature to newSignature
            components_.insert(newSignature).addElement(entity, std::move(components_.at(oldSignature).retrieveElement(entity)));

            // delete from oldSignature
            removeFrom(oldSignature, entity);
        };

        // Access a component from a specific entity
        ComponentType& retrieveFrom(Signature& signature, EntityId entity)
        {
            return components_.at(signature).retrieveElement(entity);
        };

        // Access components for a specific signature
        std::vector<std::vector<ComponentType>*> retrieveFor(Signature& signature)
        {
            std::vector<std::vector<ComponentType>*> returnVector{};

            for (auto it : components_)
            {
                // check if provided signature is subset of signature key
                if ((it->first & signature) == signature)
                {
                    returnVector.push_back(&(it->second.retrieveAllElements()));
                    
                }
            }

            return returnVector;
        };

        // Access all components
        std::vector<std::vector<ComponentType>*> retrieveAllComponents()
        {
            std::vector<std::vector<ComponentType>*> returnVector{};

            for (auto it : components_)
            {
                    returnVector.push_back(&(it->second.retrieveAllElements()));
            }

            return returnVector;
        };

    private:
        std::unordered_map<Signature, ContiguousContainer<Id, ComponentType>> components_;
    };

}

#endif
