#include "ui.h"

#include "ui_grid.h"
#include "util/colors.h"

void ui_demo() {
}

UI::UI(Render2D &renderer_2d)
    : renderer_2d(renderer_2d) {
    root = UIContainer(0, 0);
}

void UI::draw() {
    root.draw(renderer_2d, 0, 0);
}

void UI::attach(UIElement &e) {
    root.attach(e);
}

void UI::detach(UIElement &e) {
    root.detach(e);
}

void UI::enable()
{
    root.enable();
}

void UI::disable()
{
    root.disable();
}
