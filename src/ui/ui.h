#pragma once

#include "render2d.h"
#include "ui_container.h"

class UI {
public:
    UI();
    void draw();
    void attach(UIElement &e);
    void detach(UIElement &e);
    void enable();  // Enables visibility of top level UI.
    void disable(); // Disables visibliity
    void enable_all();  // Enables visibility of all contained UI elts.
    void disable_all(); // Disables visibliity.
protected:
    bool enabled = true;    // by default, UI elements are shown.z
private:
    Render2D renderer_2d;
    UIContainer root;
};
