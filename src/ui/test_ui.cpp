#include "test_ui.h"

#include "util/colors.h"

TestUI::TestUI(int grid_width, int grid_height)
    : UI(), // explicit call base class dflt constructor
      ui_grid(0, 0, grid_width*grid_height, grid_width, 5, 5, color::white),
      rect(0, 0, 5, 5, color::windwaker_green) {
	for (int i = 0; i < grid_height; ++i)
	{
		for (int j = 0; j < grid_width; ++j)
		{
			ui_grid.attach_at(i, j, rect);
		}
	}
	attach(ui_grid);
}
