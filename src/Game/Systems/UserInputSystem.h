#ifndef IG20240531194629
#define IG20240531194629

#include "Cursor.h"
#include "GameCamera.h"
#include "Hero.h"
#include "Map.h"
#include "UserInputComponent.h"
#include <raylib.h>

namespace UserInputSystem
{
    void setDefaultInputMappings(UserInputComponent& userInputComponent);

    void takeInput(
        UserInputComponent& userInputComponent,
        bool isCursorActive);

    void triggerAction(
        UserInputComponent& userInputComponent,
        Hero& hero,
        Cursor const& cursor,
        Map const& map,
        GameCamera const& gameCamera);
};

#endif
