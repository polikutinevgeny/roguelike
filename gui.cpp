#include "gui.hpp"
#include "engine.hpp"

namespace rogue {

static const int PANEL_HEIGHT = 7;
static const int BAR_WIDTH = 20;

Gui::Gui() {
    con_ = new TCODConsole(engine.width, PANEL_HEIGHT);
}

Gui::~Gui() {
    delete con_;
}

void Gui::Render() {
    con_->setDefaultBackground(TCODColor::black);
    con_->clear();
    RenderBar(1, 1, BAR_WIDTH, "HP", engine.player->hp, engine.player->max_hp, TCODColor::lightRed, TCODColor::darkerRed);
    TCODConsole::blit(con_, 0, 0, engine.width, PANEL_HEIGHT, TCODConsole::root, 0, engine.height - PANEL_HEIGHT);
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