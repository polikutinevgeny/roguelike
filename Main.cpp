#include "libtcod.hpp"
#include "game_map.hpp"
#include "engine.hpp"
#include "actors.hpp"

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main() {
    rogue::Engine engine(100, 60);
    //_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF); _ASSERTE(_CrtCheckMemory());
    while (!TCODConsole::isWindowClosed()) {
        engine.Update();
        engine.Render();
        TCODConsole::flush();
    }
    return 0;
}