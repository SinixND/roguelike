#ifndef SYSTEMS_H_20240110222501
#define SYSTEMS_H_20240110222501

#include "Components.h"
#include "EntityId.h"
#include "System.h"

namespace snd
{
    class SControl
        : public System<CPosition, CRenderOffset, TControlled>
    {
    public:
        void action(EntityId entityId);

        SControl(ECS* ecs)
            : System<CPosition, CRenderOffset, TControlled>(ecs)
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
