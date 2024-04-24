#ifndef IG20240316224041
#define IG20240316224041

class AttackComponent
{
public:
    auto range() const -> int { return range_; }
    void setRange(int range) { range_ = range; }

    auto damage() const -> int { return damage_; }
    void setDamage(int damage) { damage_ = damage; }

    explicit AttackComponent(int range, int damage)
        : range_(range)
        , damage_(damage)
    {
    }

private:
    int range_{};
    int damage_{};
};

#endif