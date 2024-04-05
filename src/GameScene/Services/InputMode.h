#ifndef IG20240409004139
#define IG20240409004139

namespace InputMode
{
    enum class Mode
    {
        mouse,
        keys,
    };

    bool isInputBlocked();
    void setInputBlocked(bool state);

    Mode inputMode();
    void setInputMode(Mode mode);
}

#endif