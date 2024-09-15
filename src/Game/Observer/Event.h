#ifndef IG20240315181752
#define IG20240315181752

enum class Event
{
    null = 0,
    // Application interaction
    windowResized,
    panelsResized,
    cameraChanged,
    cursorPositionChanged,
    colorThemeChange,
    // Game events
    actionFinished,
    actionInProgress,
    heroMoved,
    heroPositionChanged,
    nextLevel,
    previousLevel,
};

#endif
