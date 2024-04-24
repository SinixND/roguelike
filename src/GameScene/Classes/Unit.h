#ifndef IG20240211230403
#define IG20240211230403

#include "Attack.h"
#include "Graphic.h"
#include "Movement.h"
#include "Position.h"
#include "VisibilityID.h"

class Unit
{
public:
    MovementComponent movementComponent;
    PositionComponent positionComponent;
    GraphicComponent graphicComponent;
    AttackComponent attackComponent;

public:
    auto isSelected() const -> bool { return isSelected_; }
    void setIsSelected(bool state) { isSelected_ = state; }

    auto isSolid() const -> bool { return isSolid_; }

    auto visibilityID() const -> VisibilityID { return visibilityID_; }
    void setVisibilityID(VisibilityID visibilityID) { visibilityID_ = visibilityID; }

    auto visionRange() const -> int { return visionRange_; }
    void setVisionRange(int visionRange) { visionRange_ = visionRange; }

    explicit Unit(
        PositionComponent positionComponent,
        GraphicComponent graphicComponent,
        MovementComponent movementComponent,
        VisibilityID visibilityID,
        int visionRange,
        AttackComponent attackComponent)
        : movementComponent(movementComponent)
        , positionComponent(positionComponent)
        , graphicComponent(graphicComponent)
        , attackComponent(attackComponent)
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