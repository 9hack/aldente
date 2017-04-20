#pragma once

#include "ui_element.h"

#include <string>

class UITextNode : public UIElement
{
public:
    UITextNode(UIElement *parent, UIPosition pos, bool interactable, glm::vec3 color, std::string text, float scale);
    void draw(Render2D *renderer2D) override;
};
