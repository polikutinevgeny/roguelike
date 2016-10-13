#include "libtcod.hpp"
#include "game_map.hpp"
#include "engine.hpp"
#include "actors.hpp"

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

rogue::Engine rogue::engine(100, 70);

int main() {
    //_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF); _ASSERTE(_CrtCheckMemory());
    while (!TCODConsole::isWindowClosed()) {
        rogue::engine.Update();
        rogue::engine.Render();
        TCODConsole::flush ();
    }
    return 0;
}