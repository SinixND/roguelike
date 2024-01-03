#ifndef ENTITYMANAGER_H_20231220225105
#define ENTITYMANAGER_H_20231220225105

#include "ComponentTypeId.h"
#include "EntityId.h"
#include "IdManager.h"
#include "Signature.h"
#include <iostream>
#include <unordered_map>

namespace snd
{
    class EntityManager
    {
    public:
        EntityId request()
        {
            EntityId newId{
                idManager_.requestId()};

            entityToSignature_.insert(std::make_pair(newId, Signature()));

            return newId;
        };

        void remove(EntityId entityId)
        {
            idManager_.suspendId(entityId);
            entityToSignature_.erase(entityId);
        };

        void setComponent(EntityId entityId, ComponentTypeId componentTypeId)
        {
            entityToSignature_.at(entityId).set(componentTypeId);
            std::cout << "Set EntitySig: " << entityToSignature_.at(entityId) /*.getString()*/ << "\n";
        };

        void resetComponent(EntityId entity, ComponentTypeId componentTypeId)
        {
            entityToSignature_.at(entity).reset(componentTypeId);
        };

        Signature* getSignature(EntityId entity)
        {
            return &entityToSignature_.at(entity);
        };

    private:
        IdManager idManager_;
        std::unordered_map<EntityId, Signature> entityToSignature_;
    };
}

#endif
