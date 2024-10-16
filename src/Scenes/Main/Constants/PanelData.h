#ifndef IG20240531180915
#define IG20240531180915

#include "GameFont.h"
#include <raygui.h>

namespace PanelData
{
    //* Panels (sorted by descending priority)
    float constexpr PANEL_BORDER_WEIGHT{1};

    //* Info (right), Number: chars per line
    float const INFO_PANEL_WIDTH{
        static_cast<float>(
            (/*Number of chars per line*/ 15 * GameFont::fontWidth)
            + 2 * GameFont::fontWidth)};

    //* Status (top), Number: lines to print
    float const STATUS_PANEL_HEIGHT{
        static_cast<float>(
            (/*Number of lines to print*/ 1 * (1.5f * GameFont::fontHeight))
            + (0.5f * GameFont::fontHeight))};

    //* Log (bottom)
    float const LOG_PANEL_HEIGHT{
        static_cast<float>(
            (/*Number of lines to print*/ 4 * (1.5f * GameFont::fontHeight))
            + (0.5f * GameFont::fontHeight))};
}

#endif
