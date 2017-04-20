#pragma once

#include "ui_element.h"

#include <string>

class UIImageNode : public UIElement
{
public:
    UIImageNode(UIElement *parent, UIPosition pos, UIDims dims, bool interactable, glm::vec3 color, GLuint textureID);
    void draw(Render2D *renderer2D) override;
};
