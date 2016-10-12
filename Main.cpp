#include "libtcod.hpp"
#include "game_map.hpp"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main() {
    TCODConsole::initRoot(80, 50, "Rogue-like game", false);
    rogue::Game game (80, 50);
    game.PutPlayer (40, 25);
    game.PutPrincess (50, 30);
    TCODConsole::root->clear();
    game.Render();
    TCODConsole::flush();
    while (!TCODConsole::isWindowClosed()) {
        TCOD_key_t key;
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
        switch (key.vk) {
            case TCODK_UP: game.MovePlayer (0, -1); break;
            case TCODK_DOWN: game.MovePlayer (0, 1); break;
            case TCODK_LEFT: game.MovePlayer (-1, 0); break;
            case TCODK_RIGHT: game.MovePlayer (1, 0); break;
            default:break;
        }
        game.MoveMonsters();
        TCODConsole::root->clear ();
        game.Render ();
        TCODConsole::flush ();
    }
    return 0;
}