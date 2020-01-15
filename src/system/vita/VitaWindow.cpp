#include "VitaWindow.h"
#include "version.h"
#include "system/Log.h"
#include "system/Paths.h"
#include "../vita/VitaGraphicsContext.h"

#include <assert.h>

const std::string LOG_INPUT_TAG = "input";

VitaWindow::VitaWindow()
    : gcx()
    , default_gamepad_mapping({
        {(uint16_t)(SCE_CTRL_UP & 0x0000FFFF), {InputType::GAME_HARDDROP, InputType::MENU_UP}},
        {(uint16_t)(SCE_CTRL_DOWN & 0x0000FFFF), {InputType::GAME_SOFTDROP, InputType::MENU_DOWN}},
        {(uint16_t)(SCE_CTRL_LEFT & 0x0000FFFF), {InputType::GAME_MOVE_LEFT, InputType::MENU_LEFT}},
        {(uint16_t)(SCE_CTRL_RIGHT & 0x0000FFFF), {InputType::GAME_MOVE_RIGHT, InputType::MENU_RIGHT}},
        {(uint16_t)(SCE_CTRL_SQUARE & 0x0000FFFF), {InputType::GAME_ROTATE_LEFT}},
        {(uint16_t)(SCE_CTRL_CROSS & 0x0000FFFF), {InputType::MENU_OK}},
        {(uint16_t)(SCE_CTRL_CIRCLE & 0x0000FFFF), {InputType::GAME_ROTATE_RIGHT, InputType::MENU_CANCEL}},
        {(uint16_t)(SCE_CTRL_LTRIGGER & 0x0000FFFF), {InputType::GAME_HOLD}},
        {(uint16_t)(SCE_CTRL_RTRIGGER & 0x0000FFFF), {InputType::GAME_HOLD}},
        {(uint16_t)(SCE_CTRL_START & 0x0000FFFF), {InputType::GAME_PAUSE}},
    })
{
    deviceId = 0;
    device.name = "psvita";
    device.type = DeviceType::GAMEPAD;
    device.buttonmap = default_gamepad_mapping;
    device.eventmap = toEventMap(device.buttonmap);
    device_map = {{deviceId, device}}; 
    
    memset(&pad, 0, sizeof(pad));
    memset(&prevPad, 0, sizeof(prevPad));
}

void VitaWindow::toggleFullscreen()
{
}

void VitaWindow::requestScreenshot(const std::string& path)
{
}

void VitaWindow::setInputConfig(const std::map<DeviceName, DeviceData>& known)
{
    // known_mappings = known;
    // auto& kb_buttonmap = device_maps.at(-1).buttonmap;
    // kb_buttonmap = knownButtonmapForDeviceName(device_maps.at(-1).name);
    // if (kb_buttonmap.empty())
    //     kb_buttonmap = default_keyboard_mapping;
    // device_maps.at(-1).eventmap = toEventMap(kb_buttonmap);
}

std::map<DeviceName, DeviceData> VitaWindow::createInputConfig() const
{
    return {{device.name, device}};
}

const DeviceMap& VitaWindow::connectedDevices() const
{
    return device_map;
}

std::string VitaWindow::buttonName(DeviceID device_id, uint16_t raw_key) const
{
    return "<unknown>";
}

void VitaWindow::setKeyBinding(DeviceID device_id, InputType inputevent, uint16_t raw_device_key)
{
    if(device_id == device.id)
    {
        device.eventmap[inputevent] = {raw_device_key};
        device.buttonmap = toButtonMap(device.eventmap);
    }
}

std::vector<Event> VitaWindow::collectEvents()
{
    std::vector<Event> output;

    sceCtrlPeekBufferPositive(deviceId, &pad, 1);

    // Check all the keys individually in the input buffer
    for (const auto& entry : device.buttonmap) {

        auto curButton = entry.first;
        if((pad.buttons & curButton) && !(prevPad.buttons & curButton)) {
            // Trigger all the events bound to that button
            for (const auto& input_event : device.buttonmap[curButton]) {
                output.emplace_back(InputEvent(input_event, true, deviceId));
            }
        }
        if(!(pad.buttons & curButton) && (prevPad.buttons & curButton)) {
            // Trigger all the events bound to that button
            for (const auto& input_event : device.buttonmap[curButton]) {
                output.emplace_back(InputEvent(input_event, false, deviceId));
            }
        }
    }
        
    prevPad = pad;
    
    return output;
}

void VitaWindow::showErrorMessage(const std::string& title, const std::string& content)
{
}
