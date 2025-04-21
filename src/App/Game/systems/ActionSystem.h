#ifndef IG20250420205317
#define IG20250420205317

struct Hero;
struct Map;
struct Cursor;
struct GameCamera;
enum class InputId;

namespace ActionSystem
{
    /// Select/Prepare action
    void update(
        Hero& heroIO,
        Map const& map,
        InputId currentInput,
        Cursor const& cursor,
        GameCamera const& gameCamera
    );
}

#endif
