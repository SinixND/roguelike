#ifndef _20240211230403
#define _20240211230403

#include "GameObject.h"
#include "Movement.h"
#include <cstddef>

class Unit : public GameObject
{
public:
    bool selected() { return selected_; };
    void setSelected(bool selected) { selected_ = selected; };
    void toggleSelected() { selected_ = !selected_; };

    Movement& move() { return move_; };

private:
    bool selected_{};
    Movement move_{};
};

#endif