#ifndef CTEXTURE_H_20240128153537
#define CTEXTURE_H_20240128153537

#include "Component.h"
#include <raylib.h>

struct CTexture
    : public snd::Component<CTexture>
{
public:
    Texture2D* get() { return texture_; };
    void setTexture(Texture2D* texture) { texture_ = texture; };

    CTexture(Texture2D* texture)
        : texture_(texture){};

private:
    Texture2D* texture_;
};

#endif