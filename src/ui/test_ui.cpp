#include "test_ui.h"

#include "util/colors.h"

TestUI::TestUI(int num_cols, int num_rows, float aspect)
    : UI(), // explicit call base class dflt constructor
      ui_grid(0, 0, 30.f * aspect, 80.f, num_cols*num_rows, num_cols, 9, 9, color::indian_red),
      rect(0, 0, 9, 9, color::windwaker_green),
      info_panel(0, 80.f),
      info_rect(0, 0, 30.f * aspect, 20.f, color::ocean_blue)
{
    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            ui_grid.attach_at(i, j, rect);
        }
    }
    attach(ui_grid);

    info_panel.attach(info_rect);
    attach(info_panel);
}
