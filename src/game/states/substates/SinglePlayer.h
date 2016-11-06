#pragma once

#include "game/Transition.h"
#include "system/Event.h"

#include <array>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>


class AppContext;
class GraphicsContext;
class Font;
class Music;
class SinglePlayState;
class SoundEffect;
class TextPopup;


namespace SubStates {
namespace SinglePlayer {

enum class StateType : uint8_t {
    FADE_IN,
    COUNTDOWN,
    GAME_RUNNING,
    PAUSED,
    FINISHED,
};

class State {
public:
    virtual ~State() {}
    virtual StateType type() const = 0;
    virtual void update(SinglePlayState&, const std::vector<Event>&, AppContext&) = 0;
    virtual void draw(SinglePlayState&, GraphicsContext&) const {};
};

namespace States {

class FadeIn : public State {
public:
    FadeIn();
    StateType type() const { return StateType::FADE_IN; }
    void update(SinglePlayState&, const std::vector<Event>&, AppContext&) final;
    void draw(SinglePlayState&, GraphicsContext&) const final;

private:
    Transition<uint8_t> alpha;
};

class Countdown : public State {
public:
    Countdown(AppContext& app);
    StateType type() const { return StateType::COUNTDOWN; }
    void update(SinglePlayState&, const std::vector<Event>&, AppContext&) final;

private:
    uint8_t current_idx;
    Transition<void> timer;

    std::array<std::shared_ptr<SoundEffect>, 3> sfx_countdown;
};

class Pause : public State {
public:
    Pause(AppContext&);
    StateType type() const { return StateType::PAUSED; }
    void update(SinglePlayState&, const std::vector<Event>&, AppContext&) final;
};

class Gameplay : public State {
public:
    Gameplay(SinglePlayState&, AppContext&);
    StateType type() const { return StateType::GAME_RUNNING; }
    void update(SinglePlayState&, const std::vector<Event>&, AppContext&) final;
    void draw(SinglePlayState&, GraphicsContext&) const final;

private:
    enum class ScoreType : uint8_t {
        CLEAR_SINGLE,
        CLEAR_DOUBLE,
        CLEAR_TRIPLE,
        CLEAR_PERFECT,
        MINI_TSPIN,
        CLEAR_MINI_TSPIN_SINGLE,
        TSPIN,
        CLEAR_TSPIN_SINGLE,
        CLEAR_TSPIN_DOUBLE,
        CLEAR_TSPIN_TRIPLE,
        SOFTDROP,
        HARDDROP
    };

    bool gameover;
    std::shared_ptr<Music> music;
    std::shared_ptr<SoundEffect> sfx_onhold;
    std::shared_ptr<SoundEffect> sfx_onlevelup;
    std::array<std::shared_ptr<SoundEffect>, 4> sfx_onlineclear;
    std::shared_ptr<SoundEffect> sfx_onlock;
    std::shared_ptr<SoundEffect> sfx_onrotate;
    std::shared_ptr<Font> font_popuptext;
    bool texts_need_update;

    Transition<void> pending_levelup_msg;
    std::vector<std::unique_ptr<TextPopup>> textpopups;

    const unsigned lineclears_per_level;
    int lineclears_left;
    std::stack<Duration> gravity_levels;
    unsigned current_level;
    unsigned current_score;
    std::map<ScoreType, unsigned> score_table;
    std::map<ScoreType, const std::string> score_name;
    ScoreType previous_lineclear_type;

    void addNextPiece(SinglePlayState& parent);
    void registerObservers(SinglePlayState& parent);
};

} // namespace States
} // namespace SinglePlayer
} // namespace SubStates
