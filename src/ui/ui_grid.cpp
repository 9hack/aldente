#include "ui_grid.h"

#include "util/colors.h"

UIGrid::~UIGrid() {
    for (unsigned int i = 0; i < attach_points.size(); ++i)
        delete attach_points[i];
}

UIGrid::UIGrid(float start_x, float start_y,
        int num_elements, int columns,
        float element_width, float element_height,
        glm::vec3 grid_bg_color,
        float border_width,
        float inter_padding,
        float selection_halo_padding)
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

    // Create grid background element and add to children
    grid_bg = UIRectangle(start_x, start_y,
                          grid_width, grid_height,
                          grid_bg_color);
    children.push_back(&grid_bg);

    // Build empty attachment points across the grid.
    float elt_start_x = start_x + border_width;
    float elt_start_y = start_y + grid_height - border_width - element_height;
    int curr_elt = 0;
    for (unsigned int row = 0; row < rows; ++row) {
        for (unsigned int col = 0; col < columns; ++col) {
            float adjusted_x = elt_start_x + (total_element_width * col);
            float adjusted_y = elt_start_y - (total_element_height * row);

            UIContainer *attach_point = new UIContainer(adjusted_x, adjusted_y);
            // Add to both children (for rendering), and attach_points (for attachment)
            attach_points.push_back(attach_point);
            children.push_back(attach_point);

            UIElement *selection_halo =
                new UIRectangle(-selection_halo_padding, -selection_halo_padding,
                        element_width + selection_halo_padding * 2,
                        element_height + selection_halo_padding * 2,
                        color::windwaker_sand);
            attach_point->attach(*selection_halo);

            curr_elt++;
            if (curr_elt == num_elements) break; // finished doing all the elements!
        }
    }
}

void UIGrid::attach_at(int row, int col, UIElement &child) {
    // Validate location
    if (row * columns + col >= num_elements) return; // log warning
    attach_points[row * columns + col]->attach(child);
}

void UIGrid::detach_at(int row, int col, UIElement &child) {
    // Validate location
    if (row * columns + col >= num_elements) return; // log warning
    attach_points[row * columns + col]->detach(child);
}
