#include "Component.h"

#include "Id.h"

namespace snd {
    Component::Component()
    {
        IdManager* idManager{std::make_unique<IdManager>()};
        id_ = idManager->requestId();
    };

    Id Component::getId(){ return id_; };
}
