#ifndef COMPONENTMANAGER_H_20240101214521
#define COMPONENTMANAGER_H_20240101214521

#include "Component.h"
#include "ComponentTypeId.h"
#include "ContiguousMap.h"
#include "EntityId.h"
#include "Signature.h"
#include <memory>
#include <unordered_map>

namespace snd
{
    class ComponentManager
    {
    public:
        // Add component to entiy
        template <typename ComponentType>
        void insert(const Signature& newSignature, EntityId entity, ComponentType component)
        {
            // get component container (and create if needed)
            auto componentContainer{
                ensureComponentTypeContainer<ComponentType>(newSignature)};

            // add to signature
            componentContainer->insert(entity, component);
        }

        void updateSignature(ComponentTypeId componentTypeId, const Signature& oldSignature, const Signature& newSignature, EntityId entity)
        {
            // get old component container
            auto oldComponentContainer{
                getComponentContainer(componentTypeId, oldSignature)};

            // get old component container
            auto newComponentContainer{
                getComponentContainer(componentTypeId, newSignature)};

            // assign component to new container
            std::cout << "Move component \nto   signature " << newSignature /*.getString()*/ << "\n";
            // newComponentContainer->insert(entity, oldComponentContainer.get());
            newComponentContainer->insert(entity, *oldComponentContainer->get(entity));

            // remove component from old container
            std::cout << "from signature " << oldSignature /*.getString()*/ << "\n";
            // oldComponentContainer->erase(entity);
        }

        // Remove a component from an entity
        //* template <typename ComponentType>
        //* void erase(const Signature& oldSignature, EntityId entity)
        //* {
        //* // check if entity has component to be removed
        //* if (!testEntity<ComponentType>(oldSignature, entity))
        //* return;

        //* // get component container
        //* auto componentContainer{
        //* getComponentContainer<ComponentType>(oldSignature)};

        //* // remove component
        //* componentContainer.erase(entity);

        //* // delete empty signature container
        //* //* if
        //* //* signatureToComponentsByEntity_.at(oldSignature).retrieveAllElements().size())
        //* //* {
        //* //* signatureToComponentsByEntity_.erase(oldSignature);
        //* //* }
        //* }

        //* // Access a component from a specific entity
        //* ComponentType* retrieveFrom(const Signature& signature, EntityId entity)
        //* {
        //* if (!testEntity(signature, entity))
        //* return nullptr;

        //* return components_.at(signature).retrieveElement(entity);
        //* };

        //* // Access components for a specific signature
        //* std::vector<std::vector<ComponentType>*>
        //* retrieveFor(const Signature& signature)
        //* {
        //* std::vector<std::vector<ComponentType>*> returnVector{};

        //* if (!testSignature(signature))
        //* return returnVector;

        //* for (auto keyValue : components_)
        //* {
        //* // check if provided signature is subset of signature key
        //* if ((keyValue->first & signature) == signature)
        //* {
        //* returnVector.push_back(&(keyValue->second.retrieveAllElements()));
        //* }
        //* }

        //* return returnVector;
        //* };

        //* // Access all components
        //* std::vector<std::vector<ComponentType>*> retrieveAllComponents()
        //* {
        //* std::vector<std::vector<ComponentType>*> returnVector{};

        //* for (auto it : components_)
        //* {
        //* returnVector.push_back(&(it->second.retrieveAllElements()));
        //* }

        //* return returnVector;
        //* };

    private:
        std::unordered_map<ComponentTypeId, std::unordered_map<Signature, std::shared_ptr<BaseContiguousMap>>> components_{};

        template <typename ComponentType>
        auto ensureComponentTypeContainer(const Signature& signature)
        {
            // check for container existance
            if (!testSignature<ComponentType>(signature))
            {
                // make new component container
                components_[Component<ComponentType>::getId()][signature] = std::make_shared<ContiguousMap<EntityId, ComponentType>>();
            }

            return std::static_pointer_cast<ContiguousMap<EntityId, ComponentType>>(components_[Component<ComponentType>::getId()][signature]);
        }

        std::shared_ptr<BaseContiguousMap> getComponentContainer(ComponentTypeId componentTypeId, const Signature& signature)
        {
            return components_[componentTypeId][signature];
        }

        template <typename ComponentType>
        bool testEntity(const Signature& signature, EntityId entity)
        {
            if (!testSignature<ComponentType>(signature))
            {
                return false;
            }

            return components_.at(Component<ComponentType>::getId()).at(signature).tryElement(entity);
        }

        template <typename ComponentType>
        bool testSignature(const Signature& signature)
        {
            if (!testComponentTypeId<ComponentType>())
            {
                return false;
            }

            auto atComponentTypeId{
                components_[Component<ComponentType>::getId()]};

            // return components_[Component<ComponentType>::getId()].second.find(signature) != components_[Component<ComponentType>::getId()].end();
            return atComponentTypeId.find(signature) != atComponentTypeId.end();
        }

        template <typename ComponentType>
        bool testComponentTypeId()
        {
            return components_.find(Component<ComponentType>::getId()) != components_.end();
        }
    };
} // namespace snd

#endif
