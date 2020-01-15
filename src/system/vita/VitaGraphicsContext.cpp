#include "VitaGraphicsContext.h"
#include "VitaTexture.h"
#include "VitaFont.h"
#include "system/util/MakeUnique.h"

#include<memory>

const std::string LOG_TAG("video");
bool drawnThisFrame = false;

VitaGraphicsContext::VitaGraphicsContext()
{
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0x00));

    onResize(960, 544);
}

VitaGraphicsContext::~VitaGraphicsContext()
{
}

void VitaGraphicsContext::render()
{
    vita2d_end_drawing();
	vita2d_swap_buffers();
    drawnThisFrame = false;
}

unsigned short VitaGraphicsContext::screenWidth() const
{
    // debugNetPrintf(DEBUG, " VitaGraphicsContext::screenWidth: %d\n", logical_width);
    return logical_width;
}

unsigned short VitaGraphicsContext::screenHeight() const
{
    // debugNetPrintf(DEBUG, " VitaGraphicsContext::screenHeight: %d\n", logical_height);
    return logical_height;
}

float VitaGraphicsContext::getDrawScale() const
{
    return scale;
}

void VitaGraphicsContext::modifyDrawScale(float scale)
{
    this->scale = scale;
}

void VitaGraphicsContext::initFrameIfRequired() {
    if(!drawnThisFrame)
    {
        vita2d_start_drawing();
		vita2d_clear_screen();
        drawnThisFrame = true;
    }
}

std::shared_ptr<Font> VitaGraphicsContext::loadFont(const std::string& path, unsigned pt)
{
    const std::string key = path + ";" + std::to_string(pt);
    if (!font_cache.count(key))
        font_cache[key] = std::make_shared<VitaFont>((vita2d_load_font_file(path.c_str())), this, pt);
    return font_cache.at(key);
}

std::unique_ptr<Texture> VitaGraphicsContext::loadTexture(const std::string& path)
{
    vita2d_texture * tex = vita2d_load_PNG_file(path.c_str()); 
    return std::make_unique<VitaTexture>(tex, this, 0xFFFFFFFF, false);
}

std::unique_ptr<Texture> VitaGraphicsContext::loadTexture(const std::string& path, const RGBColor& tint)
{
    // TODO: Tint support
    vita2d_texture * tex = vita2d_load_PNG_file(path.c_str()); 
    return std::make_unique<VitaTexture>(tex, this, RGBA8(tint.r, tint.g, tint.b, 255), false);
}

void VitaGraphicsContext::drawFilledRect(const Rectangle& rect, const RGBColor& color)
{
    initFrameIfRequired();
    vita2d_draw_rectangle(rect.x * scale, rect.y * scale, rect.w * scale, rect.h * scale, RGBA8(color.r, color.g, color.b, 255));
}

void VitaGraphicsContext::drawFilledRect(const Rectangle& rect, const RGBAColor& color)
{
    initFrameIfRequired();
    vita2d_draw_rectangle(rect.x * scale, rect.y * scale, rect.w * scale, rect.h * scale, RGBA8(color.r, color.g, color.b, color.a));
}

void VitaGraphicsContext::onResize(int width, int height)
{
    debugNetPrintf(DEBUG, " VitaGraphicsContext::onResize: width: %d, height: %d\n", width, height);

    static constexpr float min_logical_w = 960;
    static constexpr float min_logical_h = 720;
    static constexpr float min_aspect_ratio = min_logical_w / min_logical_h;

    const float width_ratio = width / min_logical_w;
    const float height_ratio = height / min_logical_h;
    const float window_aspect_ratio = (float) width / height;

    logical_width = min_logical_w;
    logical_height = min_logical_h;
    if (window_aspect_ratio > min_aspect_ratio) {
        // wider than 4:3
        logical_width *= width / (min_logical_w * height_ratio);
    }
    else {
        // taller than 4:3
        logical_height *= height / (min_logical_h * width_ratio);
    }

    // logical_width = 960;
    // logical_height = 544;
    
    debugNetPrintf(DEBUG, " VitaGraphicsContext::onResize: logical_width: %d, logical_height: %d\n", logical_width, logical_height);

}

