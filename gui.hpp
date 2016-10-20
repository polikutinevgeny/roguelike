#pragma once
#include "libtcod.hpp"
#include "actors.hpp"
#include "loot.hpp"

namespace rogue {

class GuiCallbackInterface {
public:
    virtual Player* GetPlayer() = 0;
};

class Gui {
public:
    Gui(int gui_width, int gui_height, int main_width, int main_height, GuiCallbackInterface& engine);
    ~Gui();

    void Render(std::string message);

private:
    int gui_width_, gui_height_;
    int main_width_, main_height_;

    GuiCallbackInterface& engine_;

    TCODConsole* con_;

    void RenderBar(int x, int y, int width, const char* name, int value, int max_value, const TCODColor& bar_color, const TCODColor& back_color);

    void RenderInventory(int x, int y, std::vector<Potion*>& inventory);
};

}