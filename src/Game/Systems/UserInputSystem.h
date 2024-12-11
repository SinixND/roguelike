#ifndef IG20240531194629
#define IG20240531194629

class UserInput;
struct Hero;
class Cursor;
struct Map;
class GameCamera;

namespace UserInputSystem
{
    void setDefaultInputMappings(UserInput& userInputComponent);

    void registerInput(
        UserInput& userInputComponent,
        bool isCursorActive);

    void triggerAction(
        UserInput& userInputComponent,
        Hero& hero,
        Cursor const& cursor,
        Map& map,
        GameCamera const& gameCamera);
};

#endif
