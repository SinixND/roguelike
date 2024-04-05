#include "InputMode.h"

namespace
{
    static bool isInputBlocked{false};

    static InputMode::Mode inputMode{};
}

namespace InputMode
{
    bool isInputBlocked();
    void setInputBlocked(bool state);

    Mode inputMode();
    void setInputMode(Mode mode);
}
