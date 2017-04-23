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
           float border_width=1.0f,
           float inter_padding=0.75f,
           float selection_halo_padding=0.3f); // hardcoded defaults :(
    ~UIGrid();
    void attach_at(int row, int col, UIElement &child);
    void detach_at(int row, int col, UIElement &child);
private:
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

    std::vector<UIContainer *> attach_points; // anchors for grid positions
};
