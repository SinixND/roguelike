#ifndef IG20231203204746
#define IG20231203204746

#include "Attack.h"
#include "Entity.h"
#include "GamePhase.h"
#include "Graphic.h"
#include "IScene.h"
#include "LayerID.h"
#include "Movement.h"
#include "RenderID.h"
#include "Transformation.h"
#include "Unit.h"
#include "VisibilityID.h"
#include "World.h"

class GameScene
    : public snd::IScene
{
public:
    void initialize() override;
    void deinitialize() override;

private:
    World gameWorld_{};

    Entity cursor_{
        Transformation(),
        Graphic(
            RenderID::CURSOR,
            LayerID::UI)};

    Unit hero{
        Transformation(),
        Graphic(
            RenderID::HERO,
            LayerID::OBJECT),
        Movement(5, 50),
        VisibilityID::VISIBLE,
        20,
        Attack(
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
