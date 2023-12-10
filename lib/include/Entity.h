#ifndef ENTITY_H_20231210201820
#define ENTITY_H_20231210201820

namespace snd
{
    class Entity
    {
    public:
        int id_{};

        // to be able to put it in a std::map
        friend bool operator<(const Entity& l, const Entity& r) { return l.id_ < r.id_; }
    };
}

#endif