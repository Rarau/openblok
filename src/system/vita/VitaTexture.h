#pragma once

#include "system/Texture.h"

#include <vita2d.h>
#include <stdio.h>
#include <utility>
#include "VitaGraphicsContext.h"
#include <debugnet.h>

class VitaTexture : public Texture {
public:
    //VitaTexture(vita2d_texture *t, VitaGraphicsContext* c);
    VitaTexture(vita2d_texture *t, VitaGraphicsContext* c, unsigned int color, bool isFont);
    ~VitaTexture();
    void drawAt(int x, int y) final;
    void drawScaled(const Rectangle&) final;
    void drawPartialScaled(const Rectangle& from, const Rectangle& to) final;

    void setAlpha(uint8_t) final;
    uint8_t alpha() const final { return a; }//return tex->gxm_tex.alpha; }
    unsigned int GetColor();

    unsigned width() const final {
        return vita2d_texture_get_width(tex); 
    }
    unsigned height() const final { 
        return vita2d_texture_get_height(tex); 
    }

private:
    vita2d_texture *tex;
    VitaGraphicsContext *ctx;
    unsigned int tint = RGBA8(255,255,255,255);
    uint8_t a = 255;
    bool fromFont = false;
friend class VitaGraphicsContext;
};
