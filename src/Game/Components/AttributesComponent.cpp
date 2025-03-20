#include "AttributesComponent.h"

namespace AttributesModule
{
    int totalPoints( AttributesComponent const& attributes )
    {
        return attributes.vitality
               + attributes.agility
               + attributes.strength
               + attributes.defense;
    }
}

