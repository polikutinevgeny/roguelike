#include "libtcod.hpp"
#include "game_map.hpp"
#include "engine.hpp"
#include "drawable.hpp"

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

rogue::Engine rogue::engine(80, 50);

int main() {
    while (!TCODConsole::isWindowClosed()) {
        rogue::engine.Update();
        rogue::engine.Render();
        TCODConsole::flush ();
    }
    return 0;
}