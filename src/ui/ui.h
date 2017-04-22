#pragma once

#include "render2d.h"
#include "ui_container.h"

class UI {
public:
    UI(Render2D &renderer_2d);
    void draw();
    void attach(UIElement *e);
    void detach(UIElement *e);
    void enable();
    void disable();
private:
    Render2D renderer_2d;
    UIContainer root;
};
