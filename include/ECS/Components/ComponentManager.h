#ifndef COMPONENTMANAGER_H_20231212234818
#define COMPONENTMANAGER_H_20231212234818

#include "Component.h"
#include "ContiguousContainer.h"
#include "EntityId.h"
#include "Signature.h"
#include <cassert>
#include <functional>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

namespace snd
{
    class BaseComponentManager
    {
    public:
        virtual void updateSignature(const Signature& oldSignature, const Signature& newSignature, EntityId entity) = 0;

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
        void assignTo(Signature newSignature, EntityId entity, ComponentType component)
        {
            // add to signature
            signatureToComponentsByEntity_[newSignature].addElement(entity, component);
        };

        // Remove a component from an entity
        void removeFrom(Signature oldSignature, EntityId entity)
        {
            if (!tryEntity(oldSignature, entity))
                return;

            signatureToComponentsByEntity_.at(oldSignature).removeElement(entity);

            // delete empty signature container
            if (!signatureToComponentsByEntity_.at(oldSignature).retrieveAllElements().size())
            {
                signatureToComponentsByEntity_.erase(oldSignature);
            }
        };

        void updateSignature(const Signature& oldSignature, const Signature& newSignature, EntityId entity)
        {
            // create a pointer to the component to be moved
            auto* componentPtr{retrieveFrom(oldSignature, entity)};

            // if component does not exist
            /// if (componentPtr == nullptr)
            /// return;

            // assign component to new container
            std::cout << "Move component \nto   signature " << newSignature << "\n";
            assignTo(newSignature, entity, *componentPtr);

            // remove component from old container
            std::cout << "from signature " << oldSignature << "\n";
            removeFrom(oldSignature, entity);
        };

        // Access a component from a specific entity
        ComponentType* retrieveFrom(Signature signature, EntityId entity)
        {
            if (!tryEntity(signature, entity))
                return nullptr;

            return signatureToComponentsByEntity_.at(signature).retrieveElement(entity);
        };

        // Access components for a specific signature
        std::vector<std::vector<ComponentType>*> retrieveFor(Signature signature)
        {
            std::vector<std::vector<ComponentType>*> returnVector{};

            if (!trySignature(signature))
                return returnVector;

            for (auto keyValue : signatureToComponentsByEntity_)
            {
                // check if provided signature is subset of signature key
                if ((keyValue->first & signature) == signature)
                {
                    returnVector.push_back(&(keyValue->second.retrieveAllElements()));
                }
            }

            return returnVector;
        };

        // Access all components
        std::vector<std::vector<ComponentType>*> retrieveAllComponents()
        {
            std::vector<std::vector<ComponentType>*> returnVector{};

            for (auto it : signatureToComponentsByEntity_)
            {
                returnVector.push_back(&(it->second.retrieveAllElements()));
            }

            return returnVector;
        };

    private:
        static inline std::unordered_map<Signature, ContiguousContainer<Id, ComponentType>> signatureToComponentsByEntity_;

        bool trySignature(Signature signature)
        {
            return signatureToComponentsByEntity_.find(signature) != signatureToComponentsByEntity_.end();
        };

        bool tryEntity(Signature signature, EntityId entity)
        {
            if (!trySignature(signature))
            {
                return false;
            }

            return signatureToComponentsByEntity_.at(signature).tryElement(entity);
        };
    };

}

#endif
