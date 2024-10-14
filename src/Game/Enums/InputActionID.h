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
    moveToTarget,
    actInPlace,
    // Utility
    toggleCursor,
    // Modifiers
    mod,
};

#endif
