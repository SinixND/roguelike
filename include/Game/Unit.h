#ifndef _20240211230403
#define _20240211230403

#include "GameObject.h"
#include <cstddef>

class Unit : public GameObject
{
public:
    size_t moveRange() { return moveRange_; };
    void setMoveRange(size_t moveRange) { moveRange_ = moveRange; };

    bool selected() { return selected_; };
    void setSelected(bool selected) { selected_ = selected; };
    void toggleSelected() { selected_ = !selected_; };

private:
    size_t moveRange_{};
    bool selected_{};
};

#endif