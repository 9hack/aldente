#include "ui_grid.h"

#include "util/colors.h"

// Creates a grid with specified number of elements and dimensions of thos
// elements. Returns UI ID to the grid parent element.
UI::ID UIGrid::setup(int num_elements, int num_cols,
                   UIElement::UIDims element_dims,
                   UIElement::UIPosition bottom_left, glm::vec3 grid_bg_color,
                   int border_width, int inter_padding)
{
	int element_width = element_dims.width;
	int element_height = element_dims.height;
	int num_rows = (num_elements - 1) / num_cols + 1;
	int grid_height = 2 * border_width
					  + num_rows * element_height
					  + (num_rows - 1) * inter_padding;
	int grid_width = 2 * border_width
					  + num_cols * element_width
					  + (num_cols - 1) * inter_padding;
	int total_element_width = element_width + inter_padding;
	int total_element_height = element_height + inter_padding;

	// First create parent UI element (grid background).
	UI::ID parent_id = UI::ui->create_rectangle(
							   0,
							   bottom_left.x, bottom_left.y,
							   grid_width, grid_height,
							   false, grid_bg_color);

	// Create all grid elements.
	int elt_start_x = border_width;
	int elt_start_y = grid_height -border_width - element_height;
	int curr_elt = 0;
	for (unsigned int row = 0; row < num_rows; ++row)
	{
		for (unsigned int col = 0; col < num_cols; ++col)
		{
			int adjusted_x = elt_start_x + (total_element_width * col);
			int adjusted_y = elt_start_y - (total_element_height * row);

            UI::ID child_id = UI::ui->create_rectangle(parent_id,
                                      adjusted_x, adjusted_y,
                                      element_width, element_height,
                                      false, color::black);

            // TODO: add ID to grid selector

			curr_elt++;
			if (curr_elt == num_elements) break; // finished doing all the elements!
		}
	}
    return parent_id;
}
