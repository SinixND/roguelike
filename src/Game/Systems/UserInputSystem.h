#ifndef IG20240531194629
#define IG20240531194629

struct UserInput;
struct Hero;
class Cursor;
struct Map;
class GameCamera;

namespace UserInputSystem
{
    void setDefaultInputMappings(UserInput& userInput);

    void takeInput(
        UserInput& userInput,
        bool isCursorActive);

    void triggerAction(
        UserInput& userInput,
        Hero& hero,
        Cursor const& cursor,
        Map& map,
        GameCamera const& gameCamera);
};

#endif
