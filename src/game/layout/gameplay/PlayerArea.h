#pragma once

#include "GarbageGauge.h"
#include "WellContainer.h"
#include "game/Timing.h"
#include "game/layout/Box.h"
#include "game/components/HoldQueue.h"
#include "game/components/NextQueue.h"
#include "system/Color.h"

class AppContext;
class Font;
class GraphicsContext;


namespace Layout {
class PlayerArea : public Layout::Box {
public:
    PlayerArea(AppContext& app, bool draw_gauge);
    virtual ~PlayerArea() {}

    void update();
    void setPosition(int x, int y) override;
    void setMaxWidth(unsigned);
    void drawActive(GraphicsContext&) const;
    void drawPassive(GraphicsContext&) const;

    void updateLevelCounter(unsigned);
    void updateScore(unsigned);
    void updateGametime(Duration);
    void updateGoalCounter(unsigned);
    void updateGarbageGauge(unsigned);

    HoldQueue& holdQueue() { return hold_queue; }
    NextQueue& nextQueue() { return next_queue; }
    unsigned queuedGarbageLines() const { return garbage_gauge.lineCount(); }

    Well& well() { return ui_well.well(); };
    ::Rectangle wellBox() const { return wellbox; }
    int wellCenterX() const { return wellBox().x + wellBox().w / 2; }
    int wellCenterY() const { return wellBox().y + wellBox().h / 2; }

protected:
    bool is_narrow;
    ::Rectangle wellbox;
    WellContainer ui_well;
    const RGBAColor box_color = 0x0A0AFF80_rgba;

    static constexpr int inner_padding = 10;
    static constexpr int sidebar_width = 5 * Mino::texture_size_px;
    static constexpr int topbar_height = 4 * Mino::texture_size_px;

    std::shared_ptr<Font> font_content;
    std::shared_ptr<Font> font_content_highlight;

    std::unique_ptr<Texture> tex_hold;
    HoldQueue hold_queue;

    std::unique_ptr<Texture> tex_next;
    NextQueue next_queue;

    const bool draw_gauge;
    GarbageGauge garbage_gauge;

    ::Rectangle rect_level;
    std::unique_ptr<Texture> tex_level;
    std::unique_ptr<Texture> tex_level_counter;

    ::Rectangle rect_score;
    std::unique_ptr<Texture> tex_score;
    std::unique_ptr<Texture> tex_score_counter;

    ::Rectangle rect_goal;
    std::unique_ptr<Texture> tex_goal;
    std::unique_ptr<Texture> tex_goal_counter;

    std::string gametime_text;
    ::Rectangle rect_time;
    std::unique_ptr<Texture> tex_time_counter;

    void calcWellBox();

    std::function<void()> layout_fn;
    void calcWideLayout();
    void calcNarrowLayout();

    std::function<void(GraphicsContext&)> draw_fn_active;
    std::function<void(GraphicsContext&)> draw_fn_passive;
    void drawWideActive(GraphicsContext&) const;
    void drawWidePassive(GraphicsContext&) const;
    void drawNarrowActive(GraphicsContext&) const;
    void drawNarrowPassive(GraphicsContext&) const;
};
} // namespace Layout