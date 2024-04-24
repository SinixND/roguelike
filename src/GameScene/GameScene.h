#ifndef IG20231203204746
#define IG20231203204746

#include "Attack.h"
#include "GameObject.h"
#include "GamePhase.h"
#include "Graphic.h"
#include "IScene.h"
#include "LayerID.h"
#include "Movement.h"
#include "Position.h"
#include "RenderID.h"
#include "Unit.h"
#include "VisibilityID.h"
#include "World.h"

class GameScene
    : public snx::IScene
{
public:
    void initialize() override;
    void deinitialize() override;

private:
    World gameWorld_{};

    GameObject cursor_{
        PositionComponent(),
        GraphicComponent(
            RenderID::CURSOR,
            LayerID::UI)};

    Unit hero{
        PositionComponent(),
        GraphicComponent(
            RenderID::HERO,
            LayerID::OBJECT),
        MovementComponent(5, 50),
        VisibilityID::VISIBLE,
        20,
        AttackComponent(
            1,
            1)};

    GamePhase gamePhase_{};

    bool isInputBlocked_{false};

private:
    void processInput() override;
    void updateState() override;
    void renderOutput() override;
    void postOutput() override;
};

#endif
