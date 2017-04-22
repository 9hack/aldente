#include "ui.h"

#include "ui_grid.h"
#include "util/colors.h"

void ui_demo() {
    UIGrid uigrid = UIGrid(14, 4, {50, 50}, {0, 0}, color::windwaker_green);
}

UI::UI(Render2D &renderer_2d)
    : renderer_2d(renderer_2d) {
    root = new UIContainer(0, 0);
}

UI::~UI {
    delete root;
}

void UI::draw() {
    root->draw(renderer_2d);
}

void UI::attach(UIElement *e) {
    root->attach(e);
}

void UI::detach(UIElement *e) {
    root->detach(e);
}

void UI::enable()
{
    root->enable();
}

void UI::disable()
{
    root->disable();
}
