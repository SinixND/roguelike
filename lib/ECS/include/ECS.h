#ifndef ECS_H_20231215005456
#define ECS_H_20231215005456

#include "AttributeManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Id.h"

namespace snd
{
    typedef snd::Id Mask;

    /*
    template <typename AttributeType>
    class ECS
    {
    public:
        // Managers
        EntityManager entityManager;
        AttributeManager<AttributeType> attributeManager;

        // Entities
        Entity createEntity(){};
        void removeEntity(){};

        // Attributes
        void assignAttribute(Entity entity, AttributeType attribute){};
        AttributeType& getAttribute(Entity entity){};
        void removeAttribute(Entity entity){};
        void iterateAll(std::function<void(AttributeType attribute)> lambda){};
        Id getAttributeTypeId();

        // Logic

        ECS(){};
        private:
        std::map<Entity, Mask> entityMask;

    };
    */
}

#endif