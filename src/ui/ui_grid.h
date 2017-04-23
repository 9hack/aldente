#pragma once

#include "ui_container.h"
#include "ui_rectangle.h"
#include "ui_halo_container.h"

#include <glm/glm.hpp>

class UIGrid : public UIContainer {
public:
    enum Direction { UP, RIGHT, DOWN, LEFT };

    UIGrid(float start_x, float start_y,
           int num_elements, int columns,
           float element_width, float element_height,
           glm::vec3 grid_bg_color,
           float border_width=1.0f,
           float inter_padding=0.75f,
           float selection_halo_padding=0.3f); // hardcoded defaults :(
    ~UIGrid();
    void attach_at(int row, int col, UIElement &child);
    void detach_at(int row, int col, UIElement &child);
    void draw(Render2D &renderer_2d,
        float offset_x, float offset_y) override;
    void enable() override;
    void disable() override;
private:
    void move_selection(Direction d);
    void toggle_current_selection_halo();

    int num_elements;
    int columns, rows; // grid dimensions
    float element_width, element_height;
    glm::vec3 grid_bg_color;
    float border_width; // border padding around grid
    float inter_padding; // padding between elts
    float selection_halo_padding; // width of selection halo

    float grid_width, grid_height; // size of entire grid rectangle
    float total_element_width, total_element_height; // elt size incl padding

    UIRectangle grid_bg;

    int selection_row, selection_col;
};
