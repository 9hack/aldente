#pragma once

#include "ui_element.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

class UIImageNode : public UIElement {
public:
    UIImageNode(float start_x, float start_y,
                float width, float height,
                glm::vec3 color, GLuint texture_id)
        : UIElement(start_x, start_y),
          width(width), height(height),
          color(color), texture_id(texture_id) {}

    void draw(Render2D &renderer_2d, float offset_x, float offset_y) override;
private:
    float width, height;
    glm::vec3 color;
    GLuint texture_id;
};
