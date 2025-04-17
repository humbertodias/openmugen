#include "global.h"

// Main object of the game
CGame mugen;

// only for VC compiler
#ifdef WIN32
// only include this when compiling for windows
#include "windows.h"
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#endif

#ifdef WIN32
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#ifdef __EMSCRIPTEN__
void RunOneFrame() {
    mugen.CheckSDL();
    mugen.m_Timer.UpdateTimer();
    mugen.m_SdlManager.m_VideoSystem.Clear();

    switch (mugen.nGameType) {
        case GFIGHTGAME:
            mugen.m_FightEngine.RunEngine();
            break;
        case GMENU:
            // lógica de menu
            break;
    }

    mugen.m_SdlManager.m_VideoSystem.Draw();

    if (!mugen.bGame) {
        emscripten_cancel_main_loop();
        mugen.Quit();
    }
}
#endif

char globalStr[1024];

int main(int argc, char** argv) {
    try {
        mugen.InitGame();

#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(RunOneFrame, 0, 1);
#else
        mugen.RunGame();
        mugen.Quit();
#endif
        mugen.Quit();
    } catch (CError& e) {
        mugen.Quit();
        // windows only , change this for other OS
    }

    return 0;
}
