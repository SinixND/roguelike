#ifndef IG20240531180915
#define IG20240531180915

#include "GameFont.h"
#include <raygui.h>

namespace PanelData
{
    // Panels (sorted by descending priority)
    float constexpr PANEL_BORDER_WEIGHT{1};

    // Info (right); Number: chars per line
    float const INFO_PANEL_WIDTH{
        static_cast<float>(
            (/*Number of chars per line*/ 20 * GameFont::fontSize)
            + GameFont::fontSize)};

    // Status (top); Number: lines to print
    float const STATUS_PANEL_HEIGHT{
        static_cast<float>(
            (/*Number of lines to print*/ 2 * (1.5f * GameFont::fontSize))
            + (0.5f * GameFont::fontSize))};

    // Log (bottom);
    float const LOG_PANEL_HEIGHT{
        static_cast<float>(
            (/*Number of lines to print*/ 6 * (1.5f * GameFont::fontSize))
            + (0.5f * GameFont::fontSize))};
}

#endif
