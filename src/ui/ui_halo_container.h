#pragma once

#include "ui_container.h"
#include "ui_rectangle.h"

class UIHaloContainer : public UIContainer {
public:
    UIHaloContainer(float start_x, float start_y,
                    float element_width, float element_height,
                    float selection_halo_padding);
    void do_selection() override; // not needed, but for clarity
private:
    UIRectangle halo;
};
