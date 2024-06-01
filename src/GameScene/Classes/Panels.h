#ifndef IG20240531174000
#define IG20240531174000

#include "raylibEx.h"

class Panels
{
    RectangleEx tileInfo_{};
    RectangleEx info_{};
    RectangleEx status_{};
    RectangleEx log_{};
    RectangleEx map_{};

public:
    void init();

    RectangleEx const& tileInfo() { return tileInfo_; }
    RectangleEx const& info() { return info_; }
    RectangleEx const& status() { return status_; }
    RectangleEx const& log() { return log_; }
    RectangleEx const& map() { return map_; }

    void drawLogPanelContent();
    void drawPanelBorders();
};

#endif