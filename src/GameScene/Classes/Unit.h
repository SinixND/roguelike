#ifndef IG20240211230403
#define IG20240211230403

#include "Attack.h"
#include "GameObject.h"
#include "Graphic.h"
#include "Movement.h"
#include "Transformation.h"
#include "VisibilityID.h"

class Unit : public GameObject
{
public:
    Movement movement;
    Attack attack;

public:
    bool isSelected() const { return isSelected_; }
    void setIsSelected(bool state) { isSelected_ = state; }

    bool isSolid() const { return isSolid_; }

    VisibilityID visibilityID() const { return visibilityID_; }
    void setVisibilityID(VisibilityID visibilityID) { visibilityID_ = visibilityID; }

    int visionRange() const { return visionRange_; }
    void setVisionRange(int visionRange) { visionRange_ = visionRange; }

    explicit Unit(
        Transformation position,
        Graphic graphic,
        Movement movement,
        VisibilityID visibilityID,
        int visionRange,
        Attack attack)
        : GameObject(position, graphic)
        , movement(movement)
        , attack(attack)
        , visibilityID_(visibilityID)
        , visionRange_(visionRange)
    {
    }

private:
    bool isSelected_{false};
    bool const isSolid_{true};
    VisibilityID visibilityID_{};
    int visionRange_{};
};

#endif