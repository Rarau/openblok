#pragma once

#include "system/GraphicsContext.h"


#include <SDL2pp/SDL2pp.hh>
#include <vita2d.h>
#include <map>


/// A graphics context implementation, based on Vita2d
class VitaGraphicsContext: public GraphicsContext {
public:
    VitaGraphicsContext();
    ~VitaGraphicsContext();

    void render() final;
    unsigned short screenWidth() const final;
    unsigned short screenHeight() const final;

    float getDrawScale() const final;
    void modifyDrawScale(float scale) final;

    std::shared_ptr<Font> loadFont(const std::string& path, unsigned pt) final;
    std::unique_ptr<Texture> loadTexture(const std::string& path) final;
    std::unique_ptr<Texture> loadTexture(const std::string& path, const RGBColor& tint) final;

    void drawFilledRect(const Rectangle& rect, const RGBColor& color) final;
    void drawFilledRect(const Rectangle& rect, const RGBAColor& color) final;

    void initFrameIfRequired();
    float scale = 1.0;

    void onResize(int width, int height);

private:
    std::map<std::string, std::shared_ptr<Font>> font_cache;
    int logical_width = 960;
    int logical_height = 544;
};
