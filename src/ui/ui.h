#pragma once

#include "render2d.h"
#include "ui_container.h"

class UI {
public:
    UI();
    void draw();
    void attach(UIElement &e);
    void detach(UIElement &e);
    void enable();  // Enables visibility of all contained UI elts.
    void disable(); // Disables visibliity.
private:
    Render2D renderer_2d;
    UIContainer root;
};
