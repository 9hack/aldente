#pragma once

#include "ui.h"
#include "ui_element.h"
#include "ui_grid_element.h"

class UIGrid
{
public:
    UIGrid(int num_elements, int num_cols,
                   UIElement::UIDims element_dims,
                   UIElement::UIPosition bottom_left, glm::vec3 grid_bg_color,
                   int border_width=10, int inter_padding=10);
	~UIGrid();
	UIGridElement *get_current_selection();

	void move_selection_left();
	void move_selection_right();
	void move_selection_up();
	void move_selection_down();
private:
    void toggle_selection_halo();

	UI::ID grid_id;
	std::vector<std::vector<UIGridElement *>> grid; // (0, 0) is top left
	int selection_row, selection_col;
	int num_rows, num_cols;
	int grid_width, grid_height;
	int num_elements;
};
