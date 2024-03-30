#ifndef IG20240316224041
#define IG20240316224041

class Attack
{
public:
    int range() const { return range_; }
    void setRange(int range) { range_ = range; }

    int damage() const { return damage_; }
    void setDamage(int damage) { damage_ = damage; }

    Attack(int range, int damage)
        : range_(range)
        , damage_(damage)
    {
    }

private:
    int range_{};
    int damage_{};
};

#endif