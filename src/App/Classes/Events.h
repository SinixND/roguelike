#ifndef IG20240315181752
#define IG20240315181752

#include <string>
#include <unordered_map>

enum class EventId
{
    NONE = 0,
    CHANGE_COLOR_THEME,
    CURSOR_POSITION_CHANGED,
    CURSOR_TOGGLE,
    HERO_MOVED,
    HERO_POSITION_CHANGED,
    INTERRUPT_MOVEMENT,
    MAP_CHANGE,
    MULTIFRAME_ACTION_ACTIVE,
    MULTIFRAME_ACTION_DONE,
    NEXT_LEVEL,
    PREVIOUS_LEVEL,
    WINDOW_RESIZED,
};

inline std::unordered_map<EventId, std::string> const eventIdToName{
    { EventId::NONE, "None" },
    { EventId::CHANGE_COLOR_THEME, "Change color theme" },
    { EventId::CURSOR_POSITION_CHANGED, "Cursor position changed" },
    { EventId::CURSOR_TOGGLE, "Cursor toggle" },
    { EventId::HERO_MOVED, "Hero moved" },
    { EventId::HERO_POSITION_CHANGED, "Hero position changed" },
    { EventId::INTERRUPT_MOVEMENT, "Interrupt movement" },
    { EventId::MAP_CHANGE, "Map change" },
    { EventId::MULTIFRAME_ACTION_ACTIVE, "Multiframe action active" },
    { EventId::MULTIFRAME_ACTION_DONE, "Multiframe action done" },
    { EventId::NEXT_LEVEL, "Next level" },
    { EventId::PREVIOUS_LEVEL, "Previous level" },
    { EventId::WINDOW_RESIZED, "Window resized" },
};

#endif
