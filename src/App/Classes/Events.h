#ifndef IG20240315181752
#define IG20240315181752

#include <string>
#include <unordered_map>

enum class EventId
{
    NONE = 0,
    //* Application interaction
    WINDOW_RESIZED,
    CAMERA_CHANGED,
    CURSOR_TOGGLE,
    CURSOR_POSITION_CHANGED,
    MAP_CHANGE,
    CHANGE_COLOR_THEME,
    //* Game events
    MULTIFRAME_ACTION_DONE,
    MULTIFRAME_ACTION_ACTIVE,
    HERO_MOVED,
    HERO_POSITION_CHANGED,
    NEXT_LEVEL,
    PREVIOUS_LEVEL,
    INTERRUPT_MOVEMENT,
};

inline std::unordered_map<EventId, std::string> eventNames{
    { EventId::NONE, "None" },
    { EventId::WINDOW_RESIZED, "Window resized" },
    { EventId::CAMERA_CHANGED, "Camera changed" },
    { EventId::CURSOR_TOGGLE, "Cursor toggle" },
    { EventId::CURSOR_POSITION_CHANGED, "Cursor position changed" },
    { EventId::MAP_CHANGE, "Map change" },
    { EventId::CHANGE_COLOR_THEME, "Change color theme" },
    { EventId::MULTIFRAME_ACTION_DONE, "Multiframe action done" },
    { EventId::MULTIFRAME_ACTION_ACTIVE, "Multiframe action active" },
    { EventId::HERO_MOVED, "Hero moved" },
    { EventId::HERO_POSITION_CHANGED, "Hero position changed" },
    { EventId::NEXT_LEVEL, "Next level" },
    { EventId::PREVIOUS_LEVEL, "Previous level" },
    { EventId::INTERRUPT_MOVEMENT, "Interrupt movement" }
};

#endif
