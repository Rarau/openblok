// OpenBlok
// Copyright (C) 2016  Mátyás Mustoha
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "version.h"
#include "game/AppContext.h"
#include "game/GameState.h"
#include "game/Timing.h"
#include "game/states/InitState.h"
#include "system/Log.h"
#include "system/Paths.h"
#include "system/util/MakeUnique.h"

#include <algorithm>
#include <chrono>
#include <memory>
//#include <thread>
#include <psp2/kernel/threadmgr.h>
#include <assert.h>
#include <debugnet.h>
#include <psp2/ctrl.h>

const std::string LOG_MAIN = "main";
const std::string LOG_HELP = "help";

#define ip_server "192.168.1.60"
#define port_server 18194

int w = 920;
int h = 544;
SceCtrlData pad;


int main(int argc, const char** argv)
{
    int ret;
	ret = debugNetInit(ip_server,port_server,DEBUG);
	debugNetPrintf(DEBUG,"OpenBlok debug level %d\n",ret);

    printf("OpenBlok, created by Mátyás Mustoha, ");
    Log::info(LOG_MAIN) << "OpenBlok, created by Mátyás Mustoha, " << game_version << "\n";

    for (int arg_i = 1; arg_i < argc; arg_i++) {
        std::string arg = argv[arg_i];
        if (arg == "-v" || arg == "--version")
            return 0;
        else if (arg == "--help") {
            Log::info(LOG_HELP) << "Usage:\n";
            Log::info(LOG_HELP) << "  -v, --version            Display the version number then quit\n";
            Log::info(LOG_HELP) << "  --help                   Display this help then quit\n";
            Log::info(LOG_HELP) << "  --data <dir>             Load game resources from the <dir> directory\n";
            return 0;
        }
        else if (arg == "--data") {
            if (++arg_i >= argc) {
                Log::error(LOG_MAIN) << "'--data' requires a directory as parameter!\n";
                return 1;
            }
            Paths::changeDataDir(argv[arg_i]);
        }
        else {
            Log::error(LOG_MAIN) << "Unknown parameter '" << arg << "'.\n";
            return 1;
        }
    }

    Paths::changeDataDir("app0:/data");

    AppContext app;
    if (!app.init())
        return 1;


    try { app.states().emplace(std::make_unique<InitState>(app)); }
    catch (const std::exception& err) {
        app.window().showErrorMessage(err.what());
        return 1;
    }


    auto frame_starttime = std::chrono::steady_clock::now();
    auto frame_planned_endtime = frame_starttime + Timing::frame_duration;
    auto gametime_delay = Timing::frame_duration; // start with an update
    app.gcx().modifyDrawScale(0.78);
    while (!app.window().quitRequested()) {
        try {
            while (gametime_delay >= Timing::frame_duration && !app.states().empty()) {
                auto events = app.window().collectEvents();
                app.states().top()->update(events, app);
                gametime_delay -= Timing::frame_duration;
            }
            if (app.states().empty())
                break;
            

            app.states().top()->draw(app.gcx());

            // sceCtrlPeekBufferPositive(0, &pad, 1);

            // if(pad.buttons & SCE_CTRL_UP)
            // {
            //     debugNetPrintf(DEBUG,"SCE_CTRL_UP\n");
            //     h++;
            // }
            // if(pad.buttons & SCE_CTRL_DOWN)
            // {
            //     debugNetPrintf(DEBUG,"SCE_CTRL_DOWN\n");
            //     h--;
            // }
            // if(pad.buttons & SCE_CTRL_RIGHT)
            // {
            //     debugNetPrintf(DEBUG,"SCE_CTRL_RIGHT\n");
            //     w++;
            // }
            // if(pad.buttons & SCE_CTRL_LEFT)
            // {
            //     debugNetPrintf(DEBUG,"SCE_CTRL_LEFT\n");
            //     w--;
            // }
            // app.gcx().onResize(w, h);

            app.gcx().render();


        }
        catch (const std::exception& err) {
            app.window().showErrorMessage(err.what());
            return 1;
        }

        auto lag = std::max(std::chrono::steady_clock::now() - frame_planned_endtime, Duration::zero());
        gametime_delay += Timing::frame_duration + lag;

        // max frame rate limiting
        //std::this_thread::sleep_until(frame_planned_endtime);
	    sceKernelDelayThread(16666);
        frame_starttime = std::chrono::steady_clock::now();
        frame_planned_endtime = frame_starttime + Timing::frame_duration;
    }

    // save input config on exit
    const auto mappings = app.window().createInputConfig();
    app.inputconfig().save(mappings, Paths::config() + "input.cfg");

    return 0;
}
