#pragma once

#include "ui.h"
#include "ui_element.h"

class UIGrid
{
public:
    UI::ID setup(int num_elements, int num_cols,
                   UIElement::UIDims element_dims,
                   UIElement::UIPosition bottom_left, glm::vec3 grid_bg_color,
                   int border_width=10, int inter_padding=10);
private:
	UIElement::UIPosition bottom_left;
	int border_width;
	int num_elements, num_cols;
	int inter_padding;
	int element_width, element_height;
    glm::vec3 grid_bg_color;
};
