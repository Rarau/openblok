#include "VitaFont.h"

#include "VitaTextureText.h"
#include "system/util/MakeUnique.h"
#include <vector>
#include <assert.h>
#include <vita2d.h>
#include "VitaGraphicsContext.h"

VitaFont::VitaFont(vita2d_font *f, VitaGraphicsContext* c, unsigned int pt) : font(f), ctx(c), size(pt)
{}

std::unique_ptr<Texture> VitaFont::renderText(const std::string& text, const RGBColor& color, TextAlign align)
{
    return renderText(text, RGBAColor {color.r, color.g, color.b, 255}, align);
}

std::unique_ptr<Texture> VitaFont::renderText(const std::string& text, const RGBAColor& color, TextAlign align)
{
    int w = 0;
    int h = 0;
    ctx->initFrameIfRequired();
    vita2d_font_text_dimensions(font, size, text.c_str(), &w, &h);
    // debugNetPrintf(DEBUG, " VitaFont::renderText -> h:%d w:%d :%s\n", h, w, text.c_str());
    
    w = (w % 2) != 0 ? w + 1 : w;
    h = (h % 2) != 0 ? h + 1 : h;

    if (w < 2) {
        w = 2;
    }

    if(h < 2) {
        h = 2;
    }
    //vita2d_texture *t = vita2d_create_empty_texture_rendertarget(w, h, SCE_GXM_TEXTURE_FORMAT_A8B8G8R8);
    vita2d_texture *t = vita2d_create_empty_texture(w, h);
    vita2d_start_drawing_advanced(t, SCE_GXM_SCENE_FRAGMENT_SET_DEPENDENCY);
    vita2d_font_draw_text(font, 0, 0, RGBA8(color.r, color.g, color.b, color.a), size, text.c_str());
    //vita2d_end_drawing();

    if(t == nullptr) {
        // debugNetPrintf(ERROR, " VitaFont::renderText => nullptr!\n");
        t = vita2d_create_empty_texture(w, h);
    }
    return std::make_unique<VitaTextureText>(text, size, t, font, ctx, RGBA8(color.r, color.g, color.b, color.a));
}
