#include "VitaTexture.h"

#pragma once

#include "system/Texture.h"

#include <vita2d.h>
#include <stdio.h>
#include <utility>
#include "VitaGraphicsContext.h"
#include <debugnet.h>

class VitaTextureText : public Texture {
public:
    VitaTextureText(std::string text, unsigned int size, vita2d_texture *t, vita2d_font* font, VitaGraphicsContext* c, unsigned int color);
    ~VitaTextureText();
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
    vita2d_font* f;
    std::string text;
    unsigned int size;
friend class VitaGraphicsContext;
};
