#ifndef IG20250420205317
#define IG20250420205317

struct Hero;
struct Map;
struct Cursor;
struct GameCamera;
struct Vector2I;
enum class InputId;

namespace ActionSystem
{
    [[nodiscard]]
    Hero const& handleInputToAdjacentTarget(
        Hero& hero,
        Map const& map,
        Vector2I const& target
    );

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
