#include "WellBox.h"

#include "game/components/rotations/SRS.h"
#include "system/GraphicsContext.h"


namespace Layout {

WellBox::WellBox(AppContext&)
{
    m_well.setRotationFn(std::make_unique<Rotations::SRS>()); // TODO: make a shared rotation store

    bounding_box.w = wellWidth() + border_width * 2;
    bounding_box.h = wellHeight() + border_width * 2;

    setPosition(0, 0);
}

void WellBox::setPosition(int x, int y)
{
    bounding_box.x = x;
    bounding_box.y = y;

    border_left = { x, y, border_width, height() };
    border_right = { x + width() - border_width, y, border_width, height() };
    border_top = { x + border_width, y, width() - border_width * 2, border_width };
    border_bottom = { x + border_width, y + height() - border_width,
                      width() - border_width * 2, border_width };
}

void WellBox::drawBase(GraphicsContext& gcx) const
{
    m_well.drawBackground(gcx, x() + border_width, y() + border_width);

    static const auto boardborder_color = 0x1A3A8A_rgb;
    gcx.drawFilledRect(border_left, boardborder_color);
    gcx.drawFilledRect(border_right, boardborder_color);
    gcx.drawFilledRect(border_top, boardborder_color);
    gcx.drawFilledRect(border_bottom, boardborder_color);
}

void WellBox::drawContent(GraphicsContext& gcx) const
{
    m_well.drawContent(gcx, x() + border_width, y() + border_width);
}

} // namespace Layout
