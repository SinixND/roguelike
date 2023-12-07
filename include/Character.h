#ifndef UNIT_H_20231203215305
#define UNIT_H_20231203215305

#include "Renderable.h"
#include <raylib.h>

struct Vector2;

namespace snd
{
    class Character
        : public Renderable
    {
    public:
        Character(Vector2 position, float size, Color color);

        Vector2& getPosition();
        void setPosition(Vector2 position);
        void move(Vector2 direction, float distance);

        void render() override;

    private:
        Vector2 position_{0, 0};
        float size_;
        Color color_;
    };
}

#endif