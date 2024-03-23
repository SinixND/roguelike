#ifndef IG20240211230403
#define IG20240211230403

#include "Attack.h"
#include "Entity.h"
#include "Graphic.h"
#include "Movement.h"
#include "Transformation.h"
#include "VisibilityID.h"

class Unit : public Entity
{
public:
    Movement movement;
    Attack attack;

public:
    bool isSelected() const { return isSelected_; };
    void setIsSelected(bool isSelected) { isSelected_ = isSelected; };

    bool isSolid() const { return isSolid_; };

    VisibilityID visibilityID() const { return visibilityID_; };
    void setVisibilityID(VisibilityID visibilityID) { visibilityID_ = visibilityID; };

    size_t visionRange() const { return visionRange_; };
    void setVisionRange(size_t visionRange) { visionRange_ = visionRange; };

    Unit(Transformation position, Graphic graphic, Movement movement, VisibilityID visibilityID, int visionRange, Attack attack)
        : Entity(position, graphic)
        , movement(movement)
        , attack(attack)
        , visibilityID_(visibilityID)
        , visionRange_(visionRange){};

private:
    bool isSelected_{false};
    const bool isSolid_{true};
    VisibilityID visibilityID_;
    size_t visionRange_;
};

#endif