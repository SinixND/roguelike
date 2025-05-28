#ifndef IG20250528102723
#define IG20250528102723

struct Game;
enum class InputId;

namespace LevelUpSystem
{
    [[nodiscard]]
    Game const& update(
        Game& game,
        InputId currentInputId
    );
}

#endif
