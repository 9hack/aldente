#pragma once

#include "ui.h"

#include "ui_grid.h"
#include "ui_rectangle.h"

class TestUI : public UI {
public:
    TestUI(int grid_width, int grid_height);
private:
    UIGrid ui_grid;
    UIRectangle rect;
};
