#include "ui_grid.h"

#include "util/colors.h"

UIGrid::~UIGrid() {
    for (unsigned int i = 0; i < children.size(); ++i)
        delete children[i];
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
    selection_halo_padding(selection_halo_padding),
    selection_row(0), selection_col(0) {

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

    // Build empty attachment points across the grid.
    float elt_start_x = start_x + border_width;
    float elt_start_y = start_y + grid_height - border_width - element_height;
    int curr_elt = 0;
    for (unsigned int row = 0; row < rows; ++row) {
        for (unsigned int col = 0; col < columns; ++col) {
            float adjusted_x = elt_start_x + (total_element_width * col);
            float adjusted_y = elt_start_y - (total_element_height * row);

            UIHaloContainer *attach_point =
                new UIHaloContainer(adjusted_x, adjusted_y,
                    element_width, element_height, selection_halo_padding);
            children.push_back(attach_point);

            curr_elt++;
            if (curr_elt == num_elements) break; // finished doing all the elements!
        }
    }

    // First item in grid is selected by default (0,0)
    toggle_current_selection_halo();
}

void UIGrid::attach_at(int row, int col, UIElement &child) {
    // Validate location
    if (row * columns + col >= num_elements) return; // log warning
    children[row * columns + col]->attach(child);
}

void UIGrid::detach_at(int row, int col, UIElement &child) {
    // Validate location
    if (row * columns + col >= num_elements) return; // log warning
    children[row * columns + col]->detach(child);
}

void UIGrid::draw(Render2D &renderer_2d,
        float offset_x, float offset_y) {
    grid_bg.draw(renderer_2d, offset_x, offset_y);
    UIContainer::draw(renderer_2d, offset_x, offset_y);
}

void UIGrid::enable() {
    grid_bg.enable();
    UIContainer::enable();
}

void UIGrid::disable() {
    grid_bg.disable();
    UIContainer::disable();
}

void UIGrid::toggle_current_selection_halo() {
    children[selection_row * columns + selection_col]->do_selection();
}

void UIGrid::move_selection(Direction d) {
    switch (d) {
        case Direction::UP:
            selection_row = glm::max(0, selection_row-1);
            break;
        case Direction::RIGHT:
            selection_col = glm::min(columns-1, selection_col+1);
            break;
        case Direction::DOWN:
            selection_row = glm::min(rows-1, selection_row+1);
            break;
        case Direction::LEFT:
            selection_col = glm::max(0, selection_col-1);
            break;
        default:
            break;
    }
}
