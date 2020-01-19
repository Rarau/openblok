#include "VitaTexture.h"
#include "VitaGraphicsContext.h"
#include <assert.h>
#include <vita2d.h>


VitaTexture::VitaTexture(vita2d_texture *t, VitaGraphicsContext* c, unsigned int color, bool isFont) : ctx(c), tint(color)
{
    tex = t;
    a = 255;
    fromFont = isFont;
}

VitaTexture::~VitaTexture() {
    vita2d_free_texture(tex);
}

void VitaTexture::drawAt(int x, int y)
{
    if(fromFont){
        return;
    }
    //printf("VitaTexture::drawAt\n");
    ctx->initFrameIfRequired();
    vita2d_draw_texture_tint(tex, x * ctx->scale, y * ctx->scale, GetColor());
    //vita2d_draw_texture_tint(tex, x, y, GetColor());
}

unsigned int VitaTexture::GetColor() {
    // #define RGBA8(r,g,b,a) ((((a)&0xFF)<<24) | (((b)&0xFF)<<16) | (((g)&0xFF)<<8) | (((r)&0xFF)<<0))

    return (tint & 0x00FFFFFF) | (0xFF & alpha())<<24;
}

void VitaTexture::drawScaled(const Rectangle& rect)
{
    if(fromFont){
        return;
    }
    //printf("VitaTexture::drawScaled\n");
    ctx->initFrameIfRequired();
    float xScale = rect.w / (float)width();
    float yScale = rect.h / (float)height();

    vita2d_draw_texture_tint_scale(tex, rect.x * ctx->scale, rect.y * ctx->scale, xScale * ctx->scale, yScale * ctx->scale, GetColor());
    //vita2d_draw_texture_tint_scale(tex, rect.x, rect.y, xScale * ctx->scale, yScale * ctx->scale, GetColor());
}

void VitaTexture::drawPartialScaled(const Rectangle& from, const Rectangle& to)
{
    if(fromFont){
        return;
    }
    //printf("VitaTexture::drawPartialScaled\n");
    ctx->initFrameIfRequired();
    float xScale = ctx->scale * (to.w / width());
    float yScale = ctx->scale * (to.y / height());
    vita2d_draw_texture_tint_part_scale(tex, to.x * ctx->scale, to.y * ctx->scale, from.x, from.y, 
        from.w, from.h, xScale, yScale,
        GetColor());
}

void VitaTexture::setAlpha(uint8_t alpha)
{
    this->a = alpha;
}
