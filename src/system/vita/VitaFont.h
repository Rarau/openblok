#pragma once

#include "system/Font.h"
#include <vita2d.h>
#include <utility>
#include "VitaGraphicsContext.h"

class VitaFont : public Font {
public:
    VitaFont(vita2d_font *f, VitaGraphicsContext *c, unsigned int pt);
    std::unique_ptr<Texture> renderText(const std::string&, const RGBColor&, TextAlign) final;
    std::unique_ptr<Texture> renderText(const std::string&, const RGBAColor&, TextAlign) final;

private:
    static uint32_t pixelformat;
    vita2d_font *font;
    VitaGraphicsContext* ctx;
    unsigned int size;
friend class VitaGraphicsContext;
};
