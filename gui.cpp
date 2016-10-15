#include "gui.hpp"

namespace rogue {

Gui::Gui(int gui_width, int gui_height, int main_width, int main_height, GuiCallbackInterface& engine) 
    : engine_(engine), gui_width_(gui_width), gui_height_(gui_height)
    , main_width_(main_width), main_height_(main_height) 
{
    con_ = new TCODConsole(gui_width, gui_height);
}

Gui::~Gui() {
    delete con_;
}

void Gui::Render() {
    con_->setDefaultBackground(TCODColor::black);
    con_->clear();
    RenderBar(1, 1, gui_width_, "HP", engine_.GetPlayer().hp, engine_.GetPlayer().max_hp, TCODColor::lightRed, TCODColor::darkerRed);
    TCODConsole::blit(con_, 0, 0, main_width_, gui_height_, TCODConsole::root, 0, main_height_ - gui_height_);
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

}