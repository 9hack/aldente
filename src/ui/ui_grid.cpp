#include "ui_grid.h"

#include "util/colors.h"

UIGrid::UIGrid(float start_x, float start_y,
        int num_elements, int columns,
        float element_width, float element_height,
        glm::vec3 grid_bg_color,
        int border_width,
        int inter_padding,
        int selection_halo_padding)
    : UIContainer(start_x, start_y),
    num_elements(num_elements), columns(columns),
    element_width(element_width), element_height(element_height),
    grid_bg_color(grid_bg_color),
    border_width(border_width), inter_padding(inter_padding),
    selection_halo_padding(selection_halo_padding) {

    // Calculate number of rows.
    rows = (num_elements - 1) / columns + 1;
    // Calculate grid dimension.
    grid_height = 2 * border_width
                  + rows * element_height
                  + (rows - 1) * inter_padding;
    grid_width = 2 * border_width
                 + columns * element_width
                 + (columns - 1) * inter_padding;
    // Size of elements including padding
    total_element_width = element_width + inter_padding;
    total_element_height = element_height + inter_padding;

    // Create grid background element
    grid_bg = UIRectangle(start_x, start_y,
                          grid_width, grid_height,
                          grid_bg_color);
    children.push_back(&grid_bg);
}

/*
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

            curr_elt++;
            if (curr_elt == num_elements) break; // finished doing all the elements!
        }
    }
*/

void UIGrid::attach(UIElement *child) {

}

void UIGrid::detach(UIElement *child) {

}
