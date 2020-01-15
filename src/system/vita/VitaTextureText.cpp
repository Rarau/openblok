#include "VitaTextureText.h"

#include "VitaGraphicsContext.h"
#include <assert.h>
#include <vita2d.h>


VitaTextureText::VitaTextureText(std::string text, unsigned int size, vita2d_texture *t, vita2d_font* font, VitaGraphicsContext* c, unsigned int color) : ctx(c), tint(color)
{
    tex = t;
    a = 255;
    f = font;
    this->text = text;
    this->size = size;
}

VitaTextureText::~VitaTextureText() {
    vita2d_free_texture(tex);
    //vita2d_free_font(f);
}

unsigned int VitaTextureText::GetColor() {
    return (tint & 0x00FFFFFF) | (0xFF & alpha())<<24;
}

void VitaTextureText::drawAt(int x, int y)
{
    ctx->initFrameIfRequired();
    int w = 0;
    int h = 0;
    
    vita2d_font_text_dimensions(f, size, text.c_str(), &w, &h);

    vita2d_font_draw_text(f, x * ctx->scale , (y + h) * ctx->scale, GetColor(), size, text.c_str());
}

void VitaTextureText::drawScaled(const Rectangle& rect)
{
}

void VitaTextureText::drawPartialScaled(const Rectangle& from, const Rectangle& to)
{
}

void VitaTextureText::setAlpha(uint8_t alpha)
{
    this->a = alpha;
}
