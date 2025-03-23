#ifndef IG20250323230402
#define IG20250323230402

struct Cursor;
struct Game;
struct GameCamera;
enum class InputId;

namespace GameSystem
{
    [[nodiscard]]
    Game const& update(
        Game& game,
        GameCamera const& gameCamera,
        Cursor const& cursor,
        InputId currentInputId,
        float dt
    );
}

#endif
