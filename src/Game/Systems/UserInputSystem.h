#ifndef IG20240531194629
#define IG20240531194629

class UserInputComponent;
struct Hero;
class Cursor;
class Map;
class GameCamera;

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
        Map& map,
        GameCamera const& gameCamera);
};

#endif
