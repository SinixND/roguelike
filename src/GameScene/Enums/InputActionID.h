#ifndef IG20240719133200
#define IG20240719133200

enum class InputActionID
{
    none = 0,
    // Actions
    actUp,
    actLeft,
    actDown,
    actRight,
    // Modifiers
    mod,
};

// inline auto constexpr operator#(InputActionID id) noexcept
// {
// return static_cast<int>(id);
// }

#endif
