#pragma once

#include "../sdl/SDLAudioContext.h"
//#include "SDLGraphicsContext.h"
#include "../vita/VitaGraphicsContext.h"
#include "system/Window.h"

#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <psp2/ctrl.h>


class VitaWindow : public Window {
public:
    VitaWindow();

    void toggleFullscreen() final;
    void requestScreenshot(const std::string&) final;
    GraphicsContext& graphicsContext() final { return gcx; };
    AudioContext& audioContext() final { return audio; };

    std::vector<Event> collectEvents() final;
    bool quitRequested() final { return false; }

    void setInputConfig(const std::map<DeviceName, DeviceData>&) final;
    void setKeyBinding(DeviceID, InputType, uint16_t) final;
    std::map<DeviceName, DeviceData> createInputConfig() const final;
    const DeviceMap& connectedDevices() const final;
    std::string buttonName(DeviceID, uint16_t) const final;

    static void showErrorMessage(const std::string& title, const std::string& content);

private:
    VitaGraphicsContext gcx;
    SDLAudioContext audio;

    DeviceID deviceId;
    DeviceData device;
    DeviceMap device_map;
    
    const ButtonToEventsMap default_gamepad_mapping;
    std::map<uint16_t, bool> prevState;

    SceCtrlData pad;
    SceCtrlData prevPad;
};
