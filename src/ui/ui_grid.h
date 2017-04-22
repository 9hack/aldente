#pragma once

#include "ui_container.h"
#include "ui_rectangle.h"

#include <glm/glm.hpp>

class UIGrid : public UIContainer {
public:
    UIGrid(float start_x, float start_y,
           int num_elements, int columns,
           float element_width, float element_height,
           glm::vec3 grid_bg_color,
           int border_width=10,
           int inter_padding=10,
           int selection_halo_padding=5); // hardcoded padding :(
    void attach(UIElement &child) override;
    void detach(UIElement &child) override;
private:
    int num_elements;
    int columns, rows; // grid dimensions
    float element_width, element_height;
    glm::vec3 grid_bg_color;
    int border_width; // border padding around grid
    int inter_padding; // padding between elts
    int selection_halo_padding; // width of selection halo

    int grid_width, grid_height; // size of entire grid rectangle
    int total_element_width, total_element_height; // elt size incl padding

    UIRectangle grid_bg;
};
