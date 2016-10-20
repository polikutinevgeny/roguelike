#include "gui.hpp"
#include <string>

namespace rogue {

namespace {

const int BAR_WIDTH = 40;

}

Gui::Gui(int gui_width, int gui_height, int main_width, int main_height, GuiCallbackInterface& engine) 
    : engine_(engine), gui_width_(gui_width), gui_height_(gui_height)
    , main_width_(main_width), main_height_(main_height) 
{
    con_ = new TCODConsole(gui_width, gui_height);
}

Gui::~Gui() {
    delete con_;
}

void Gui::Render(std::string message) {
    con_->setDefaultBackground(TCODColor::black);
    con_->clear();
    RenderBar(1, 1, BAR_WIDTH, "HP", engine_.GetPlayer()->hp, engine_.GetPlayer()->max_hp, TCODColor::lightRed, TCODColor::darkerRed);
    RenderBar(1, 3, BAR_WIDTH, "MP", engine_.GetPlayer()->mp, engine_.GetPlayer()->max_mp, TCODColor::lightBlue, TCODColor::darkerBlue);
    RenderInventory(BAR_WIDTH + 2, 1, engine_.GetPlayer()->inventory);
    con_->print(BAR_WIDTH + 13, 1, message.c_str());
    TCODConsole::blit(con_, 0, 0, main_width_, gui_height_, TCODConsole::root, 0, 0);
}

void Gui::RenderBar(int x, int y, int width, const char * name, int value, int max_value, const TCODColor & bar_color, const TCODColor & back_color) {
    con_->setDefaultBackground(back_color);
    con_->rect(x, y, width, 1, false, TCOD_BKGND_SET);
    int bar_width = value * width / max_value;
    if (bar_width > 0) {
        con_->setDefaultBackground(bar_color);
        con_->rect(x, y, bar_width, 1, false, TCOD_BKGND_SET);
    }
    con_->setDefaultForeground(TCODColor::white);
    con_->printEx(x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER, "%s : %d/%d", name, value, max_value);
}

void Gui::RenderInventory(int x, int y, std::vector<Potion*>& inventory) {
    int i = 0;
    for (auto p : inventory) {
        con_->setChar(x + i, y, p->symbol);
        con_->setCharForeground(x + i, y, p->color);
        con_->setChar(x + i, y + 1, std::to_string(i)[0]);
        ++i;
    }
}

}