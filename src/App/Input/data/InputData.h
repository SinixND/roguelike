#ifndef IG20250402173915
#define IG20250402173915

namespace InputData
{
    //* Maximum hold time for tap event to trigger
    double constexpr MAX_TAP_TIME{ 0.3f };
    //* Minimum hold time for hold event to trigger
    double constexpr MIN_HOLD_TIME{ 0.3f };
    //* Maximum time between taps for double tap event to trigger
    double constexpr MAX_DOUBLE_TAP_TIME{ 0.3f };
}

#endif

