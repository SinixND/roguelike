#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "Components.h"
#include "EntityId.h"
#include "System.h"

namespace snd
{
    class SMouseControl
        : public System<CPosition, CRenderOffset, TMouseControlled>
    {
    public:
        void action(EntityId entityId);

        SMouseControl(ECS* ecs)
            : System<CPosition, CRenderOffset, TMouseControlled>(ecs)
        {
        }
    };

    class SMovement
        : public System<CPosition, CDirection, CRenderOffset, TKeyControlled>
    {
    public:
        void action(EntityId entityId);

        SMovement(ECS* ecs)
            : System<CPosition, CDirection, CRenderOffset, TKeyControlled>(ecs)
        {
        }
    };

    class SRenderMap
        : public System<CTexture, CPosition, CRenderOffset, TRenderMap>
    {
    public:
        void action(EntityId entityId);

        SRenderMap(ECS* ecs)
            : System<CTexture, CPosition, CRenderOffset, TRenderMap>(ecs){};
    };

    class SRenderObjects
        : public System<CTexture, CPosition, CRenderOffset, TRenderObject>
    {
    public:
        void action(EntityId entityId);

        SRenderObjects(ECS* ecs)
            : System<CTexture, CPosition, CRenderOffset, TRenderObject>(ecs){};
    };

    class SRenderUI
        : public System<CTexture, CPosition, CRenderOffset, TRenderUI>
    {
    public:
        void action(EntityId entityId);

        SRenderUI(ECS* ecs)
            : System<CTexture, CPosition, CRenderOffset, TRenderUI>(ecs){};
    };

    // System template
    /*
        class CustomSystem
            : public System<ComponentTypes>
        {
        public:
            void action(EntityId entityId)
            {
                // Get components
                // auto& component{ecs_->retrieveComponent<ComponentType>(entityId)->component_};

                // Action

            }
            CustomSystem(ECS* ecs)
                : System<ComponentTypes>(ecs)
            {
            }
        }
    */
}

#endif
