#ifndef IG20240316224041
#define IG20240316224041

#include <cstddef>

class Attack
{
public:
    size_t range() const { return range_; };
    void setRange(size_t range) { range_ = range; };

    int damage() const { return damage_; };
    void setDamage(int damage) { damage_ = damage; };

    Attack(size_t range, int damage)
        : range_(range)
        , damage_(damage)
    {
    }

private:
    size_t range_{};
    int damage_{};
};

#endif