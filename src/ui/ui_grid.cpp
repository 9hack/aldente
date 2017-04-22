#include "ui_grid.h"

#include "util/colors.h"

// Creates a grid with specified number of elements and dimensions of thos
// elements. Returns UI ID to the grid parent element.
UIGrid::UIGrid(int num_elements, int num_cols,
                   UIElement::UIDims element_dims,
                   UIElement::UIPosition bottom_left, glm::vec3 grid_bg_color,
                   int border_width, int inter_padding)
{
	int halo_padding = 4;

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

	// Saving the dimensions
	this->num_cols = num_cols;
	this->num_rows = num_rows;

    // Create grid 2D vector structure.
    for (unsigned int i = 0; i < num_rows; ++i)
    {
        grid.push_back(std::vector<UIGridElement *>(num_cols));
    }

	// First create parent UI element (grid background).
	grid_id = UI::ui->create_rectangle(
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

            UI::ID ui_container = UI::ui->create_rectangle(grid_id,
                                      adjusted_x, adjusted_y,
                                      element_width, element_height,
                                      false, color::windwaker_sand);
			UI::ID element_meat = UI::ui->create_rectangle(ui_container,
										  halo_padding, halo_padding,
										  element_width-halo_padding*2, element_height-halo_padding*2,
										  false, color::indian_red);
			UI::ui->set_enabled(ui_container, false, false);

            // Make UIGridElement
			grid[row][col] = new UIGridElement(ui_container, element_meat);

			curr_elt++;
			if (curr_elt == num_elements) break; // finished doing all the elements!
		}
	}

    // Default selection is first element
    selection_row = selection_col = 0;
    toggle_selection_halo();
}

void UIGrid::toggle_selection_halo()
{
    get_current_selection()->toggle();
}

// Returns the ID for the "element_meat" of the chosen UI Grid Element.
UIGridElement *UIGrid::get_current_selection()
{
	return grid[selection_row][selection_col];
}

void UIGrid::move_selection_left()
{
	if (selection_col > 0)
    {
        toggle_selection_halo();
        selection_col--;
        toggle_selection_halo();
    }
}

void UIGrid::move_selection_right()
{
	if (selection_col < num_cols-1)
    {
        toggle_selection_halo();
        selection_col++;
        toggle_selection_halo();
    }
}

void UIGrid::move_selection_up()
{
	if (selection_row > 0)
    {
        toggle_selection_halo();
        selection_row--;
        toggle_selection_halo();
    }
}

void UIGrid::move_selection_down()
{
	if (selection_row < num_rows-1)
    {
        toggle_selection_halo();
        selection_row++;
        toggle_selection_halo();
    }
}
