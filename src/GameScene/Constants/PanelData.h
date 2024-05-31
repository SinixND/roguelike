#ifndef IG20240531180915
#define IG20240531180915

#include <raygui.h>

// Panels (sorted by descending priority)
float constexpr PANEL_BORDER_WEIGHT{1};

// Info (right); Number: chars per line
float const INFO_PANEL_WIDTH{
    static_cast<float>(
        (10 /*Number of chars per line*/ * GuiGetStyle(DEFAULT, TEXT_SIZE))
        + GuiGetStyle(DEFAULT, TEXT_SIZE))};

// Status (top); Number: lines to print
float const STATUS_PANEL_HEIGHT{
    static_cast<float>(
        (1 /*Number of lines to print*/ * (1.5f * GuiGetStyle(DEFAULT, TEXT_SIZE)))
        + (0.5f * GuiGetStyle(DEFAULT, TEXT_SIZE)))};

// Log (bottom);
float const LOG_PANEL_HEIGHT{
    static_cast<float>(
        (3 /*Number of lines to print*/ * (1.5f * GuiGetStyle(DEFAULT, TEXT_SIZE)))
        + (0.5f * GuiGetStyle(DEFAULT, TEXT_SIZE)))};

#endif