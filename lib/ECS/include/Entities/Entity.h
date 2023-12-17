#ifndef ENTITY_H_20231210201820
#define ENTITY_H_20231210201820

#include "Id.h"

namespace snd
{
    typedef Id Entity;

    // class Entity
    //{
    // public:
    // ID id_{};

    //// to be able to put it in a std::unordered_map
    // friend bool operator<(const EntityID& l, const EntityID& r) { return l.id_ < r.id_; }
    //};
}

#endif