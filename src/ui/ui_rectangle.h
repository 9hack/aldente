#pragma once

#include "ui_element.h"

#include <glm/glm.hpp>

class UIRectangle : public UIElement
{
public:
    UIImageNode(float start_x, float start_y,
                float width, float height,
                glm::vec3 color)
        : start_x(start_x), start_y(start_y),
          width(width), height(height),
          color(color) {}

    void draw(Render2D &renderer_2d, float offset_x, float offset_y) override;
private:
    float width, height;
    glm::vec3 color;
};
