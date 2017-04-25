#pragma once

#include "ui.h"

#include "ui_grid.h"
#include "ui_rectangle.h"

class TestUI : public UI {
public:
    TestUI(int num_cols, int num_rows, float aspect);
private:
    UIGrid ui_grid;
    UIRectangle rect;
    UIContainer info_panel;
    UIRectangle info_rect;
};
