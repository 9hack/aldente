#include <audio/audio_manager.h>
#include "ui_grid.h"

#include "events.h"
#include "game/phase/phase.h"

UIGrid::~UIGrid() {
    for (unsigned int i = 0; i < children.size(); ++i)
        delete children[i];
}

UIGrid::UIGrid(float start_x, float start_y,
        float grid_width, float grid_height,
        int num_elements, int columns,
        float element_width, float element_height,
        Color grid_bg_color,
        float inter_padding,
        float selection_halo_padding,
        float grid_bg_alpha)
    : UIContainer(start_x, start_y),
    grid_width(grid_width), grid_height(grid_height),
    num_elements(num_elements), columns(columns),
    element_width(element_width), element_height(element_height),
    grid_bg_color(grid_bg_color),
    inter_padding(inter_padding),
    selection_halo_padding(selection_halo_padding),
    selection_row(0), selection_col(0) {

    // Calculate number of rows.
    rows = (num_elements - 1) / columns + 1;
    // Calculate grid border dimensions.
    h_border_padding = 0.5f * (grid_width - (columns * element_width)
                       - ((columns - 1) * inter_padding));
    v_border_padding = 0.5f * (grid_height - (rows * element_height)
                       - ((rows - 1) * inter_padding));
    // Size of elements including padding
    total_element_width = element_width + inter_padding;
    total_element_height = element_height + inter_padding;

    // Create grid background element and add to children
    grid_bg = UIRectangle(start_x, start_y,
                          grid_width, grid_height,
                          grid_bg_color, grid_bg_alpha);

    // Build empty attachment points across the grid.
    float elt_start_x = h_border_padding;
    float elt_start_y = grid_height - v_border_padding - element_height;
    int curr_elt = 0;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
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

int UIGrid::get_selection_index() {
    return selection_row * columns + selection_col;
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

    // Deselects all children except for the currently selected.
    for (int i = 0; i < num_elements; i++)
        if (i != selection_row * columns + selection_col)
            children[i]->do_selection();
}

void UIGrid::disable() {
    grid_bg.disable();
    UIContainer::disable();
}

void UIGrid::set_alpha(float alpha) {
    UIContainer::set_alpha(alpha);
}

void UIGrid::toggle_current_selection_halo() {
    children[selection_row * columns + selection_col]->do_selection();
}

void UIGrid::move_selection(Direction d) {
    toggle_current_selection_halo();
    int old_selection_row = selection_row;
    int old_selection_col = selection_col;

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

    if ((selection_row != old_selection_row) || (selection_col != old_selection_col)) {
        events::sound_effects_event(events::AudioData(AudioManager::SELECT1_SOUND, false));
    }

    toggle_current_selection_halo();
}
